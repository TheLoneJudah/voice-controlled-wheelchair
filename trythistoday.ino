#include <SoftwareSerial.h>

// === CONFIGURATION ===============================================
// Speeds can be adjusted here for safety and control
const int MAX_SPEED = 25;   // Safe forward speed
const int TURN_SPEED = 20;  // Controllable turning speed

// --- Joystick Sensitivity ---
const int JOY_THRESHOLD_LOW = 400;
const int JOY_THRESHOLD_HIGH = 600;

// === PIN DEFINITIONS =============================================
#define L_RPWM 5 // Left FWD
#define L_LPWM 6 // Left BWD (FAULTY - but we still set to 0)
#define R_RPWM 10 // Right FWD
#define R_LPWM 9 // Right BWD
#define L_EN 4
#define R_EN 3
#define JOYSTICK_X A0
#define JOYSTICK_Y A1
#define POWER_LED 13
#define NODEMCU_RX 7
#define NODEMCU_TX 8

// === GLOBAL VARIABLES =============================================
SoftwareSerial softSerial(NODEMCU_RX, NODEMCU_TX);

bool systemOn = false;
enum ControlMode { MODE_IDLE, MODE_JOYSTICK, MODE_VOICE };
ControlMode currentMode = MODE_IDLE;

int targetLeftSpeed = 0;
int targetRightSpeed = 0;
int actualLeftSpeed = 0;
int actualRightSpeed = 0;
const int RAMP_STEP = 5;
const int RAMP_DELAY = 20;

unsigned long lastActivityTime = 0;
const unsigned long inactivityLimit = 60000;
unsigned long lastRampTime = 0;

unsigned long patternStartTime = 0;
unsigned long blinkDelay = 300;
bool ledState = false;
bool isBlinking = false;

void setup() {
  Serial.begin(9600);
  softSerial.begin(9600);

  pinMode(L_RPWM, OUTPUT); pinMode(L_LPWM, OUTPUT);
  pinMode(R_RPWM, OUTPUT); pinMode(R_LPWM, OUTPUT);
  pinMode(L_EN, OUTPUT);   pinMode(R_EN, OUTPUT);
  pinMode(POWER_LED, OUTPUT);

  // Safe startup state
  digitalWrite(POWER_LED, LOW);
  disableDrivers();
  analogWrite(L_RPWM, 0); analogWrite(L_LPWM, 0);
  analogWrite(R_RPWM, 0); analogWrite(R_LPWM, 0);

  Serial.println("System Ready. Send 'PWR' to start.");
}

void loop() {
  if (!systemOn) {
    listenForPowerOn();
    return;
  }
  handleJoystick();
  handleVoice();
  updateMotorSpeeds();
  handleLEDBlinking();
  checkInactivity();
}

// === CORE LOGIC ===================================================

void updateMotorSpeeds() {
  if (millis() - lastRampTime < RAMP_DELAY) return;
  lastRampTime = millis();

  // Ramp speeds towards their targets
  if (actualLeftSpeed < targetLeftSpeed) actualLeftSpeed += RAMP_STEP;
  else if (actualLeftSpeed > targetLeftSpeed) actualLeftSpeed -= RAMP_STEP;
  if (actualRightSpeed < targetRightSpeed) actualRightSpeed += RAMP_STEP;
  else if (actualRightSpeed > targetRightSpeed) actualRightSpeed -= RAMP_STEP;

  // ### SAFETY LOCK FOR FAULTY LEFT DRIVER ###
  int leftSpeedValue = abs(actualLeftSpeed);
  leftSpeedValue = constrain(leftSpeedValue, 0, MAX_SPEED);
  if (actualLeftSpeed > 0) { // Forward is OK
    analogWrite(L_RPWM, leftSpeedValue);
    analogWrite(L_LPWM, 0);
  } else { // If backward or stop is commanded, just STOP.
    analogWrite(L_RPWM, 0);
    analogWrite(L_LPWM, 0);
  }

  // --- Right Motor (Good Driver - Full Control) ---
  int rightSpeedValue = abs(actualRightSpeed);
  rightSpeedValue = constrain(rightSpeedValue, 0, MAX_SPEED);
  if (actualRightSpeed > 0) { // Forward
    analogWrite(R_RPWM, rightSpeedValue);
    analogWrite(R_LPWM, 0);
  } else if (actualRightSpeed < 0) { // Backward
    analogWrite(R_RPWM, 0);
    analogWrite(R_LPWM, rightSpeedValue);
  } else { // Stop
    analogWrite(R_RPWM, 0);
    analogWrite(R_LPWM, 0);
  }
}

// === INPUT HANDLING ===============================================

void handleJoystick() {
  int xVal = analogRead(JOYSTICK_X);
  int yVal = analogRead(JOYSTICK_Y);

  if (xVal < JOY_THRESHOLD_LOW || xVal > JOY_THRESHOLD_HIGH || yVal < JOY_THRESHOLD_LOW || yVal > JOY_THRESHOLD_HIGH) {
    currentMode = MODE_JOYSTICK;
    lastActivityTime = millis();
    isBlinking = false;

    if (yVal > JOY_THRESHOLD_HIGH) moveForward();
    else if (yVal < JOY_THRESHOLD_LOW) moveBackward();
    else if (xVal < JOY_THRESHOLD_LOW) turnLeft();
    else if (xVal > JOY_THRESHOLD_HIGH) turnRight();
  } else {
    if (currentMode == MODE_JOYSTICK) {
      currentMode = MODE_IDLE;
      stopMotors();
    }
  }
}

void handleVoice() {
  if (softSerial.available()) {
    String cmd = softSerial.readStringUntil('\n');
    cmd.trim();
    cmd.toUpperCase();
    if (cmd.length() == 0) return;
    Serial.println("Voice Command: " + cmd);
    
    if (cmd == "POFF") { powerOff(); return; }

    currentMode = MODE_VOICE;
    lastActivityTime = millis();
    setBlinkPattern(cmd);

    if (cmd == "FWD") moveForward();
    else if (cmd == "BWD") moveBackward();
    else if (cmd == "LFT") turnLeft();
    else if (cmd == "RGT") turnRight();
    else if (cmd == "STP") stopMotors();
  }
}

// === LED & POWER FUNCTIONS =========================================

void handleLEDBlinking() {
  if (!isBlinking) {
    digitalWrite(POWER_LED, systemOn ? HIGH : LOW);
    return;
  }
  if (millis() - patternStartTime >= blinkDelay) {
    patternStartTime = millis();
    ledState = !ledState;
    digitalWrite(POWER_LED, ledState);
  }
}

void setBlinkPattern(String cmd) {
  patternStartTime = millis();
  ledState = true;
  if (cmd == "FWD") blinkDelay = 150;
  else if (cmd == "BWD") blinkDelay = 400;
  else if (cmd == "LFT") blinkDelay = 100;
  else if (cmd == "RGT") blinkDelay = 250;
  else { isBlinking = false; return; }
  isBlinking = true;
}

void listenForPowerOn() {
  if (softSerial.available()) {
    String cmd = softSerial.readStringUntil('\n');
    cmd.trim();
    cmd.toUpperCase();
    if (cmd == "PWR") {
      systemOn = true;
      enableDrivers();
      digitalWrite(POWER_LED, HIGH);
      lastActivityTime = millis();
      Serial.println("System Powered ON");
    }
  }
}

void checkInactivity() {
  if (millis() - lastActivityTime > inactivityLimit) {
    powerOff();
  }
}

void powerOff() {
  systemOn = false;
  stopMotors();
  delay(500); // Allow motors to ramp down
  disableDrivers();
  digitalWrite(POWER_LED, LOW);
  isBlinking = false;
  Serial.println("System Powered OFF");
}

void enableDrivers() {
  digitalWrite(L_EN, HIGH);
  digitalWrite(R_EN, HIGH);
}

void disableDrivers() {
  digitalWrite(L_EN, LOW);
  digitalWrite(R_EN, LOW);
}

// === MOVEMENT COMMANDS ============================================

void moveForward()  { targetLeftSpeed = MAX_SPEED;   targetRightSpeed = MAX_SPEED; }
void moveBackward() { targetLeftSpeed = -MAX_SPEED;  targetRightSpeed = -MAX_SPEED; } // Left will be ignored by safety lock
void turnLeft()     { targetLeftSpeed = -TURN_SPEED; targetRightSpeed = TURN_SPEED; } // Left will be ignored by safety lock
void turnRight()    { targetLeftSpeed = TURN_SPEED;  targetRightSpeed = -TURN_SPEED; } // This will be a pivot turn
void stopMotors()   { targetLeftSpeed = 0;           targetRightSpeed = 0; }
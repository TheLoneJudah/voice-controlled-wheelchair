# Retrofitting of a Manual Wheelchair into an Adaptive Voice-Controlled Hybrid Mobility System

**A Final Year Project by Ajibade Praise Titus (2020/9415)**
*A Project Submitted in Partial Fulfilment of the Requirements for the Award of Bachelor of Engineering (B.Eng.) in Mechatronics Engineering, College of Engineering, Bells University of Technology, Ota.*
*July 2025*

**Supervisor:** Engr. Daniel Akinwande

---

## 🎥 Project Demonstration

The included video file (`WhatsApp Video 2025-10-28 at 09.05.10_360f7bcc.mp4`) shows the retrofitted wheelchair in operation. Voice commands such as "Power on," "Move forward," "Turn left," "Turn right," and "Stop" are given via the Android application, and the wheelchair executes the movements in a real-world environment.



## 📄 Abstract

[cite_start]This report presents the design and implementation of a voice-controlled wheelchair with hybrid control, tailored specifically for paediatric users with motor impairments[cite: 1991, 2046]. [cite_start]The system integrates both voice recognition and manual control via a two-axis joystick to ensure safety, reliability, and consistent accessibility[cite: 2047, 2179, 2189]. [cite_start]Voice commands such as "move forward", "turn left", and "stop" are processed through an Android application developed using the Massachusetts Institute of Technology (MIT) App Inventor[cite: 2048, 2784]. [cite_start]This enables low-latency command execution without dependence on constant internet connectivity by using a local Wi-Fi network[cite: 2048, 3212]. [cite_start]The joystick serves as a backup in case of voice recognition failure or communication loss[cite: 2049, 3306, 3339].

[cite_start]The methodology involved constructing the control system on two microcontroller platforms: the **Arduino Uno** for hardware-level control and the **NodeMCU (ESP8266)** for wireless communication[cite: 2050, 3020, 3033, 3037]. [cite_start]Performance evaluation focused on command reliability, communication responsiveness via Wi-Fi, and mechanical robustness[cite: 2051, 3199]. [cite_start]Experimental results revealed a voice recognition accuracy of approximately 90% in quiet environments[cite: 2052, 3376]. [cite_start]Overall, the system demonstrated the feasibility of a cost-effective, dual-mode assistive mobility solution that can significantly improve autonomy and independence for children with physical disabilities[cite: 2054].

## ✨ Key Features

* [cite_start]**Hybrid Control System:** The user can control the wheelchair via two methods: voice commands or a manual joystick[cite: 2181, 2189, 3384].
* [cite_start]**Voice Control (Wireless):** A custom-built Android app (using MIT App Inventor) sends commands to the wheelchair over a local Wi-Fi network[cite: 2048, 2784, 2859, 3042].
* [cite_start]**Joystick Fail-Safe:** The joystick acts as a reliable manual override[cite: 2189]. [cite_start]If any joystick movement is detected, it automatically takes priority over voice commands for safety[cite: 3341, 3510].
* [cite_start]**Offline Local Network:** The NodeMCU ESP8266 creates its own Wi-Fi Access Point (`Wheelchair_AP`)[cite: 3212, 3533]. [cite_start]The system **does not require an internet connection** to function, making it reliable in various environments[cite: 2048].
* [cite_start]**Cost-Effective Retrofitting:** Designed as an add-on kit to convert a standard manual wheelchair [cite: 2186, 2215][cite_start], making it an affordable alternative to expensive commercial models[cite: 2287].
* [cite_start]**Paediatric Focus:** The design and objectives are specifically targeted at providing mobility for children with motor impairments like cerebral palsy or muscular dystrophy[cite: 1991, 2169, 2182].
* [cite_start]**Safety-Oriented Code:** Includes features like motor speed ramping for smooth acceleration/deceleration [cite: 3493, 3500-3502][cite_start], an inactivity timeout to automatically power off the system [cite: 3494, 3525][cite_start], and LED feedback for system status [cite: 3495-3496, 3518-3519].

## ⚙️ System Architecture

The system is built on a two-microcontroller architecture:

1.  **NodeMCU (ESP8266) - The "Communication Hub"**
    * [cite_start]**Role:** Creates a Wi-Fi Access Point (`ssid: "Wheelchair_AP"`)[cite: 3533].
    * [cite_start]**Action:** It runs a web server that listens for commands from the Android app[cite: 3537]. [cite_start]When it receives an HTTP GET request at the `/command` endpoint (e.g., `http://[IP]/command?text=FWD`), it takes the command string ("FWD") and forwards it to the Arduino Uno via a serial connection[cite: 3535].
    * **Code:** `NODEMCU.ino`

2.  **Arduino Uno - The "Main Controller"**
    * [cite_start]**Role:** Acts as the central brain of the wheelchair[cite: 2818].
    * **Inputs:**
        * [cite_start]Receives command strings (like "FWD", "LFT") from the NodeMCU via `SoftwareSerial` (pins 7, 8)[cite: 3489, 3491, 3514].
        * [cite_start]Constantly reads analog values from the `JOYSTICK_X` (A0) and `JOYSTICK_Y` (A1) pins[cite: 3491, 3509].
    * **Logic:**
        * [cite_start]If the joystick is moved, it immediately enters `MODE_JOYSTICK` and executes the joystick's command, ignoring any incoming voice commands as a safety override [cite: 3492, 3510-3511].
        * [cite_start]If a voice command is received, it enters `MODE_VOICE`[cite: 3492, 3516].
        * [cite_start]If no input is received, it enters `MODE_IDLE` and stops the motors[cite: 3492, 3513].
    * [cite_start]**Outputs:** Based on the active mode, it sets `targetLeftSpeed` and `targetRightSpeed`[cite: 3492]. [cite_start]A separate function `updateMotorSpeeds()` then smoothly ramps the `actualLeftSpeed` and `actualRightSpeed` to their targets [cite: 3500-3502] [cite_start]and sends the final PWM signals to the motor drivers [cite: 3503-3509].
    * **Code:** `trythistoday.ino`

3.  **Android App - The "User Interface"**
    * [cite_start]**Role:** Provides a simple button interface for the user to initiate voice commands[cite: 2784, 3042].
    * [cite_start]**Action:** When the user speaks, the app converts the speech to text[cite: 3049]. [cite_start]It then sends this text string (e.g., "FWD") in an HTTP request to the NodeMCU's web server[cite: 3054].
    * **Code:** `TalkToMe (1).aia`

## 🛠️ Hardware & Software

### Hardware Components
* [cite_start]**Main Controller:** Arduino Uno [cite: 2818]
* [cite_start]**Wireless Controller:** NodeMCU ESP8266 [cite: 2778]
* [cite_start]**Motors:** 2x 350W 24V DC Gear Motors (MY1016Z2) [cite: 2755]
* [cite_start]**Motor Driver:** 2x BTS7960 43A High-Power H-Bridge [cite: 2763]
* [cite_start]**Power:** 2x 12V 18Ah Sealed Lead-Acid (SLA) Batteries (wired in series for 24V) [cite: 2823, 3202]
* [cite_start]**Power Converter:** DC-DC Buck Converter (24V down to 5V for microcontrollers) [cite: 2826]
* [cite_start]**Input:** 2-axis Joystick Module (KY-023) [cite: 3491]
* [cite_start]**Frame:** Paediatric Manual Wheelchair [cite: 2749]
* [cite_start]**Drivetrain:** Chain and Sprocket System [cite: 2759, 3333-3334]
* [cite_start]**Indicator:** Power LED [cite: 3491]

### Software & Tools
* [cite_start]**Embedded:** Arduino IDE (C++) [cite: 2854, 2857]
* [cite_start]**Mobile App:** MIT App Inventor (Block coding) [cite: 2855, 2861]
* [cite_start]**Documentation:** Microsoft Excel (for calculations) [cite: 2855, 2859]

## 📂 Repository Contents

* `README.md`: This file.
* `AJIBADE PRAISE FINAL YEAR PROJECT.pdf`: The complete final year project report, including background, methodology, results, and analysis.
* `trythistoday.ino`: The main controller sketch to be uploaded to the **Arduino Uno**.
* `NODEMCU.ino`: The web server and Wi-Fi AP sketch to be uploaded to the **NodeMCU ESP8266**.
* `TalkToMe (1).aia`: The MIT App Inventor source file for the Android control application.
* `WhatsApp Video 2025-10-28 at 09.05.10_360f7bcc.mp4`: A video demonstration of the working project.
* [cite_start]*Note: Appendices B and C of the report mention Google Drive links for source codes and calculations [cite: 3486-3488].*

## 🚀 How to Set Up & Run

1.  [cite_start]**Hardware Assembly:** Assemble the hardware components as shown in the circuit schematic (Figure 3.8) in the `AJIBADE PRAISE FINAL YEAR PROJECT.pdf`[cite: 3009].
2.  **Arduino Uno:** Open `trythistoday.ino` in the Arduino IDE. Select your Arduino Uno board and COM port, then upload the sketch.
3.  **NodeMCU (ESP8266):**
    * Open `NODEMCU.ino` in the Arduino IDE.
    * Ensure you have the ESP8266 board manager installed.
    * Select your NodeMCU/ESP8266 board and its COM port, then upload the sketch.
4.  **Android App:**
    * Go to the [MIT App Inventor](https://appinventor.mit.edu/) website.
    * Click on `Projects` > `Import project (.aia) from my computer...` and select the `TalkToMe (1).aia` file.
    * Build the app and install the `.apk` file on an Android device.
5.  **Running the System:**
    * Power on the entire wheelchair system (the 24V batteries). The NodeMCU will begin broadcasting its Wi-Fi network.
    * On your Android phone, go to your Wi-Fi settings.
    * [cite_start]Connect to the Wi-Fi network named **"Wheelchair_AP"**[cite: 3533].
    * [cite_start]Enter the password: **"12345678"**[cite: 3533].
    * Once connected, open the "TalkToMe" app you installed.
    * Tap the button and speak a command. [cite_start]The first command should be **"PWR"** to activate the system, as shown in the `listenForPowerOn()` function [cite: 3523-3525].
    * [cite_start]The wheelchair is now ready to accept movement commands (FWD, BWD, LFT, RGT, STP) [cite: 3516-3518].

## ⚠️ IMPORTANT: Code Note

[cite_start]The `trythistoday.ino` code contains a **"SAFETY LOCK FOR FAULTY LEFT DRIVER"**[cite: 3502].

[cite_start]This section of the code (lines 110-117) **intentionally prevents the left motor from moving in reverse** (`actualLeftSpeed < 0`) [cite: 3504-3505]. This was implemented to compensate for a faulty hardware component during the project's development.

This means that with the code as-is:
* [cite_start]`moveBackward()` will only move the right wheel backward[cite: 3504, 3529].
* [cite_start]`turnLeft()` (which requires the left wheel to go backward) will not execute as a zero-point turn[cite: 3504, 3529].
* [cite_start]`turnRight()` (left motor forward, right motor backward) will work[cite: 3531].

If you are replicating this project with two fully functional motor drivers, you must **remove or comment out lines 114-117** in `trythistoday.ino` to restore full bidirectional control to the left motor.
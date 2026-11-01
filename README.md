# Voice-Controlled Wheelchair
**A Final Year Project by Ajibade Praise Titus (2020/9415)**
*A Project Submitted in Partial Fulfilment of the Requirements for the Award of Bachelor of Engineering (B.Eng.) in Mechatronics Engineering, College of Engineering, Bells University of Technology, Ota.*
*July 2025*

**Copyright (c) 2025 Ajibade Praise Titus. All rights reserved.** See [LICENSE](LICENSE).

**Supervisor:** Engr. Daniel Akinwande

---

## 🎥 Project Demonstration

The included video file ([`demo.mp4`](demo.mp4)) shows the retrofitted wheelchair in operation. Voice commands such as "Power on," "Move forward," "Turn left," "Turn right," and "Stop" are given via the Android application, and the wheelchair executes the movements in a real-world environment.



## 📄 Abstract

This report presents the design and implementation of a voice-controlled wheelchair with hybrid control, tailored specifically for paediatric users with motor impairments. The system integrates both voice recognition and manual control via a two-axis joystick to ensure safety, reliability, and consistent accessibility. Voice commands such as "move forward", "turn left", and "stop" are processed through an Android application developed using the Massachusetts Institute of Technology (MIT) App Inventor. This enables low-latency command execution without dependence on constant internet connectivity by using a local Wi-Fi network. The joystick serves as a backup in case of voice recognition failure or communication loss.

The methodology involved constructing the control system on two microcontroller platforms: the **Arduino Uno** for hardware-level control and the **NodeMCU (ESP8266)** for wireless communication. Performance evaluation focused on command reliability, communication responsiveness via Wi-Fi, and mechanical robustness. Experimental results revealed a voice recognition accuracy of approximately 90% in quiet environments. Overall, the system demonstrated the feasibility of a cost-effective, dual-mode assistive mobility solution that can significantly improve autonomy and independence for children with physical disabilities.

## ✨ Key Features

* **Hybrid Control System:** The user can control the wheelchair via two methods: voice commands or a manual joystick.
* **Voice Control (Wireless):** A custom-built Android app (using MIT App Inventor) sends commands to the wheelchair over a local Wi-Fi network.
* **Joystick Fail-Safe:** The joystick acts as a reliable manual override. If any joystick movement is detected, it automatically takes priority over voice commands for safety.
* **Offline Local Network:** The NodeMCU ESP8266 creates its own Wi-Fi Access Point (`Wheelchair_AP`). The system **does not require an internet connection** to function, making it reliable in various environments.
* **Cost-Effective Retrofitting:** Designed as an add-on kit to convert a standard manual wheelchair, making it an affordable alternative to expensive commercial models.
* **Paediatric Focus:** The design and objectives are specifically targeted at providing mobility for children with motor impairments like cerebral palsy or muscular dystrophy.
* **Safety-Oriented Code:** Includes features like motor speed ramping for smooth acceleration/deceleration, an inactivity timeout to automatically power off the system, and LED feedback for system status.

## ⚙️ System Architecture

The system is built on a two-microcontroller architecture:

1.  **NodeMCU (ESP8266) - The "Communication Hub"**
    * **Role:** Creates a Wi-Fi Access Point (`ssid: "Wheelchair_AP"`).
    * **Action:** It runs a web server that listens for commands from the Android app. When it receives an HTTP GET request at the `/command` endpoint (e.g., `http://[IP]/command?text=FWD`), it takes the command string ("FWD") and forwards it to the Arduino Uno via a serial connection.
    * **Code:** `NODEMCU.ino`

2.  **Arduino Uno - The "Main Controller"**
    * **Role:** Acts as the central brain of the wheelchair.
    * **Inputs:**
        * Receives command strings (like "FWD", "LFT") from the NodeMCU via `SoftwareSerial` (pins 7, 8).
        * Constantly reads analog values from the `JOYSTICK_X` (A0) and `JOYSTICK_Y` (A1) pins.
    * **Logic:**
        * If the joystick is moved, it immediately enters `MODE_JOYSTICK` and executes the joystick's command, ignoring any incoming voice commands as a safety override.
        * If a voice command is received, it enters `MODE_VOICE`.
        * If no input is received, it enters `MODE_IDLE` and stops the motors.
    * **Outputs:** Based on the active mode, it sets `targetLeftSpeed` and `targetRightSpeed`. A separate function `updateMotorSpeeds()` then smoothly ramps the `actualLeftSpeed` and `actualRightSpeed` to their targets and sends the final PWM signals to the motor drivers.
    * **Code:** `trythistoday.ino`

3.  **Android App - The "User Interface"**
    * **Role:** Provides a simple button interface for the user to initiate voice commands.
    * **Action:** When the user speaks, the app converts the speech to text. It then sends this text string (e.g., "FWD") in an HTTP request to the NodeMCU's web server.
    * **Code:** `TalkToMe (1).aia`

## 🛠️ Hardware & Software

### Hardware Components
* **Main Controller:** Arduino Uno
* **Wireless Controller:** NodeMCU ESP8266
* **Motors:** 2x 350W 24V DC Gear Motors (MY1016Z2)
* **Motor Driver:** 2x BTS7960 43A High-Power H-Bridge
* **Power:** 2x 12V 18Ah Sealed Lead-Acid (SLA) Batteries (wired in series for 24V)
* **Power Converter:** DC-DC Buck Converter (24V down to 5V for microcontrollers)
* **Input:** 2-axis Joystick Module (KY-023)
* **Frame:** Paediatric Manual Wheelchair
* **Drivetrain:** Chain and Sprocket System
* **Indicator:** Power LED

### Software & Tools
* **Embedded:** Arduino IDE (C++)
* **Mobile App:** MIT App Inventor (Block coding)
* **Documentation:** Microsoft Excel (for calculations)

## 📂 Repository Contents

* `README.md`: This file.
* `AJIBADE PRAISE FINAL YEAR PROJECT.pdf`: The complete final year project report, including background, methodology, results, and analysis.
* `trythistoday.ino`: The main controller sketch to be uploaded to the **Arduino Uno**.
* `NODEMCU.ino`: The web server and Wi-Fi AP sketch to be uploaded to the **NodeMCU ESP8266**.
* `TalkToMe (1).aia`: The MIT App Inventor source file for the Android control application.
* `demo.mp4`: A video demonstration of the working project.
* *Note: Appendices B and C of the report mention Google Drive links for source codes and calculations.*

## 🚀 How to Set Up & Run

1.  **Hardware Assembly:** Assemble the hardware components as shown in the circuit schematic (Figure 3.8) in the `AJIBADE PRAISE FINAL YEAR PROJECT.pdf`.
2.  **Arduino Uno:** Open `trythistoday.ino` in the Arduino IDE. Select your Arduino Uno board and COM port, then upload the sketch.
3.  **NodeMCU (ESP8266):**
    * Open `NODEMCU.ino` in the Arduino IDE and set your own Wi-Fi password.
    * Ensure you have the ESP8266 board manager installed.
    * Select your NodeMCU/ESP8266 board and its COM port, then upload the sketch.
4.  **Android App:**
    * Go to the [MIT App Inventor](https://appinventor.mit.edu/) website.
    * Click on `Projects` > `Import project (.aia) from my computer...` and select the `TalkToMe (1).aia` file.
    * Build the app and install the `.apk` file on an Android device.
5.  **Running the System:**
    * Power on the entire wheelchair system (the 24V batteries). The NodeMCU will begin broadcasting its Wi-Fi network.
    * On your Android phone, go to your Wi-Fi settings.
    * Connect to the Wi-Fi network named **"Wheelchair_AP"**.
    * Enter the password you set in `NODEMCU.ino`.
    * Once connected, open the "TalkToMe" app you installed.
    * Tap the button and speak a command. The first command should be **"PWR"** to activate the system, as shown in the `listenForPowerOn()` function.
    * The wheelchair is now ready to accept movement commands (FWD, BWD, LFT, RGT, STP).

## ⚠️ IMPORTANT: Code Note

The `trythistoday.ino` code contains a **"SAFETY LOCK FOR FAULTY LEFT DRIVER"**.

This section of the code (lines 93-102) **intentionally prevents the left motor from moving in reverse** (`actualLeftSpeed < 0`). This was implemented to compensate for a faulty hardware component during the project's development.

This means that with the code as-is:
* `moveBackward()` will only move the right wheel backward.
* `turnLeft()` (which requires the left wheel to go backward) will not execute as a zero-point turn.
* `turnRight()` (left motor forward, right motor backward) will work.

If you are replicating this project with two fully functional motor drivers, you must **replace lines 96-102** in `trythistoday.ino` with the same logic used for the right motor (lines 107-116) to restore full bidirectional control to the left motor.


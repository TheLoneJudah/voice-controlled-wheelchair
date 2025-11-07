# Retrofitting of a Manual Wheelchair into an Adaptive Voice-Controlled Hybrid Mobility System

**A Final Year Project by Ajibade Praise Titus (2020/9415)**
*A Project Submitted in Partial Fulfilment of the Requirements for the Award of Bachelor of Engineering (B.Eng.) in Mechatronics Engineering, College of Engineering, Bells University of Technology, Ota.*
*July 2025*


---

## 🎥 Project Demonstration

The included video file (`WhatsApp Video 2025-10-28 at 09.05.10_360f7bcc.mp4`) shows the retrofitted wheelchair in operation. Voice commands such as "Power on," "Move forward," "Turn left," "Turn right," and "Stop" are given via the Android application, and the wheelchair executes the movements in a real-world environment.

[Image of the completed wheelchair project]

## 📄 Abstract

This report presents the design and implementation of a voice-controlled wheelchair with hybrid control, tailored specifically for paediatric users with motor impairments. The system integrates both voice recognition and manual control via a two-axis joystick to ensure safety, reliability, and consistent accessibility. Voice commands such as "move forward", "turn left", and "stop" are processed through an Android application developed using the Massachusetts Institute of Technology (MIT) App Inventor. This enables low-latency command execution without dependence on constant internet connectivity by using a local Wi-Fi network. The joystick serves as a backup in case of voice recognition failure or communication loss.

The methodology involved constructing the control system on two microcontroller platforms: the **Arduino Uno** for hardware-level control and the **NodeMCU (ESP8266)** for wireless communication. Performance evaluation focused on command reliability, communication responsiveness via Wi-Fi, and mechanical robustness. Experimental results revealed a voice recognition accuracy of approximately 90% in quiet environments. Overall, the system demonstrated the feasibility of a cost-effective, dual-mode assistive mobility solution that can significantly improve autonomy and independence for children with physical disabilities.

## ✨ Key Features

* **Hybrid Control System:** The user can control the wheelchair via two methods: voice commands or a manual joystick.
* **Voice Control (Wireless):** A custom-built Android app (using MIT App Inventor) sends commands to the wheelchair over a local Wi-Fi network.
* **Joystick Fail-Safe:** The joystick acts as a reliable manual override. If any joystick movement is detected, it automatically takes priority over voice commands for safety.
* **Offline Local Network:** The NodeMCU ESP8266 creates its own Wi-Fi Access Point (`Wheelchair_AP`). The system **does not require an internet connection** to function, making it reliable in various environments.
* **Cost-Effective Retrofitting:** Designed as an add-on kit to convert

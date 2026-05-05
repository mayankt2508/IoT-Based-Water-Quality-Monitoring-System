# IoT-Based-Water-Quality-Monitoring-System

![ESP32](https://img.shields.io/badge/ESP32-100%25-blue?style=flat&logo=espressif)
![Blynk IoT](https://img.shields.io/badge/Blynk-IoT-green?style=flat&logo=blynk)
![C++](https://img.shields.io/badge/Language-C++-00599C?style=flat&logo=c%2B%2B)

## 📌 Project Overview
A low-cost, highly efficient, Internet of Things (IoT) based prototype designed for real-time water quality monitoring. This system utilizes an ESP32 microcontroller to capture live analog and digital data from an array of environmental sensors. It eliminates the limitations of traditional offline monitoring by processing data locally via an I2C LCD while simultaneously pushing live telemetry to a **Blynk IoT cloud server**. 

## ✨ Key Features
*   **Real-Time Edge Processing:** Independent local monitoring via a 16x2 I2C LCD.
*   **Cloud Telemetry:** Live Wi-Fi data transmission to the Blynk Mobile/Web dashboard.
*   **Automated Cloud Alerts:** Custom push notifications triggered when water parameters deviate from safe environmental thresholds (e.g., WHO guidelines).
*   **Custom Mathematical Calibration:** Implements a two-point calibration engine mapped directly to the ESP32's 12-bit ADC for high-precision pH readings, completely bypassing generic closed-source libraries.
*   **Temperature Compensation:** Algorithmically adjusts raw TDS voltage based on real-time water temperature to ensure accurate ppm output.

## 🛠️ Hardware Stack
*   **Microcontroller:** ESP32-WROOM-32 Development Board
*   **Sensors:** 
    *   Digital Waterproof Temperature Sensor (DS18B20)
    *   Analog TDS Sensor Meter
    *   Analog Optical Turbidity Sensor
    *   Analog pH Sensor Module (pH-4502C)
*   **Display:** 16x2 LCD with I2C Module

## 🔌 Circuit Wiring & Pinout

| Component / Sensor | Module Pin | ESP32 GPIO Pin |
| :--- | :--- | :--- |
| **I2C LCD (16x2)** | SDA / SCL | GPIO 21 / GPIO 22 |
| **DS18B20 (Temp)** | DQ (Data) | GPIO 33 |
| **TDS Sensor** | A0 (Analog) | GPIO 32 |
| **Turbidity Sensor** | A0 (Analog) | GPIO 34 |
| **pH Sensor**| Po (Analog) | GPIO 35 |

## 🚀 Software Setup & Installation
1.  Clone this repository to your local machine.
2.  Open the `.ino` file in the Arduino IDE.
3.  Install the required libraries via the Arduino Library Manager:
    *   `WiFi.h` & `WiFiClient.h`
    *   `BlynkSimpleEsp32.h`
    *   `LiquidCrystal_I2C.h`
    *   `OneWire.h` & `DallasTemperature.h`
4.  Update the Wi-Fi credentials and Blynk Authentication Token at the top of the code.
5.  Compile and upload to your ESP32 board.

## ☁️ Blynk IoT Configuration
Create four Virtual Datastreams in your Blynk Template:
*   **V0:** Temperature (`Double`, 0-50)
*   **V1:** TDS (`Integer`, 0-1000)
*   **V2:** Turbidity (`Double`, 0-5.0)
*   **V3:** pH Level (`Double`, 0-14.0)

*(Configured Automations in the Blynk Console to trigger notifications based on specific threshold limits).*

## 👨‍💻 Author
**Mayank Thakkar**
*Embedded Systems & IoT Developer*
I specialize in building custom hardware prototypes, IoT dashboards, and reverse-engineering sensor calibrations for freelance commissions and academic projects. 

* 🔗 **LinkedIn:** www.linkedin.com/in/mayank-thakkar-6545b82a6
* 📧 **Email:** thakkarmayank82@gmail.com
* 💻 **GitHub:** https://github.com/mayankt2508
  
*If you are looking to build a custom IoT solution or need help with embedded C++ development, feel free to reach out!*

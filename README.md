# Target Camera (ESP32-CAM)
A Wireless Target Camera System based off the ESP32-CAM. A combination of programming languages is used, Arduino IDE for the ESP32-CAM, Windows UWP (Visual Basic) for the client application which also uses an MJPEG library for reading the camera stream. The UWP client application displays the stream and overlays ballistic information.

#### Arduino - IDE code for the ESP32-CAM
- Battery Management - Read ADC (PIN 12) - Prior to WiFi initialization which ties up the ADC inputs on the board.
- Battery Management - Requires a voltage divider, please see the Altium Circuitmaker Schematic and PCB layout.
- OTA / Over The Air Updates - Partition Scheme: "Minimal SPIFFS (1.9MB APP with OTA/190KB SPIFFS).
- SPIFFS partition is small, is primarily used to store config data and a small image file/logo file and favicon.

#### 3D Printing - 3D Printed Parts for the ESP32-CAM Housing/Enclosure
- Main Section - Houses the ESP32-CAM and ESP32-CAM motherboard, light pipes and power switch.
- Battery Section - Houses the battery board and AA battery holder.
- Holder - Anetnna - Part is used for installing the antenna assembly.
- Holder - Nut - Part used for locating/holding the UNC nut for tripods.
- Locating Block - Screwed into the battery board, holds the ESM32-CAM motherboard in place.

Note: Recommend ABS or PETG for 3D Printing due to outdoor use.

#### UWP-Testing-VB - Simple Visual Studio 2022 Application to Read MJPEG Stream from the ESP32-CAM
This application is the testing application for reading the MJPEG stream and displaying it from the ESP32-CAM, please see information below on the required NuGet packages and configuration information. Developed using Microsoft Visual Studio 2022 and Visual Basic.

NuGet Package - Microsoft.NETCore.UniversalWindowsPlatform - Version 6.1.9
NuGet Package - MjpegProcessor - Version 1.0.0 - https://github.com/follesoe/MjpegProcessor

Visual Studio 2022 Project Configuration (Visual Basic and C#)
- Target Version - Windows 10, version 2004 (10.0; Build 19041)
- Minimum Version - Windows 10, version 1809 (10.0; Build 17753)

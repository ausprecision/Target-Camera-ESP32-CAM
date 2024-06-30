# Target Camera (ESP32-CAM)
A Wireless Target Camera System based off the ESP32-CAM. A combination of programming languages is used, Arduino IDE for the ESP32-CAM, Windows UWP (Visual Basic) for the client application which also uses an MJPEG library for reading the camera stream. The UWP client application displays the stream and overlays ballistic information.

#### Arduino - IDE code for the ESP32-CAM
- Battery Management - Reads ADC on PIN 12 - Prior to WiFi initialization which ties up the ADC inputs on the board.
- Battery Management - Requires a voltage divider, please see the Altium Circuitmaker Schematic and PCB layout.
- OTA / Over The Air Updates - Partition Scheme: "Minimal SPIFFS (1.9MB APP with OTA/190KB SPIFFS).
- SPIFFS partition is small, is primarily used to store config data and a small image file/logo file and favicon.

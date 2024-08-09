#### Arduino - IDE code for the ESP32-CAM (Version 1 - STABLE RELEASE)
- Battery Management - Read ADC (PIN 12) - Prior to WiFi initialization which ties up the ADC inputs on the board.
- Battery Management - Requires a voltage divider, please see the Altium Circuitmaker Schematic and PCB layout.
- OTA / Over The Air Updates - Partition Scheme: "Minimal SPIFFS (1.9MB APP with OTA/190KB SPIFFS).
- SPIFFS partition is small, is primarily used to store config data and a small image file/logo file and favicon.

- MJPEG Streaming Server - Accessible via the web interface on http://192.168.4.1
- OTA Update Server - Accessible via the web interface on http://192.168.4.1:8090

#### Version 1 - STABLE RELEASE:
This is the initial release of the ESP32-CAM Target Camera Arduino code, it has basic functionality though is stable and can be used via the web interface or via the Visual Studio Code via this respository (UWP-Testing-VB).

Battery management is only read upon boot/startup as once WiFi is enabled and running it ties up the ADC inputs so further readings can not be obtained. It is a limitation of the board, the run time on  4xAA batteries is quite long so this should not be a problem for those using the system for only a few hours at a time - e.g. 4 hours of target shooting. The overall run time on batteries with the MJPEG streaming server running has not been tested as of yet.
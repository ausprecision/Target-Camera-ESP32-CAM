KB Article: AP100001
Product(s): AUS Precision Disk Operating System
Version(s): AP-DOS:1.00
Keyword(s): apdos
Last Modified: 23-SEP-2024
-------------------------------------------------------------------------------
The information in this article applies to:

- AUS Precision AP-DOS Operating System Version 1.00 
-------------------------------------------------------------------------------

SUMMARY
=======

The following information was taken from the AP-DOS 1.00 README.TXT file. This
file is referred to in the AP-DOS1 Concise User's Guide. This file refers to
how AP-DOS interacts with the hardware and software of your ESP32 based target
camera system hardware.


1. HARDWARE COMPATIBILITY WITH AP-DOS
=====================================
1.1 AP-DOS is compatible with any ESP32 based SoC Processor, though is ideally
suited for the ESP32S series processors that are used with various ESP32-CAM
boards from many manufacturers.

1.2 The firmware running AP-DOS uses general purpose input/output (GPIO Pins)
for various functions. These include reading battery levels from the 
analogue to digital converter (ADC) running on the board itself as well as
turning various LED lights on the board on or off.

1.3 Target Camera Systems supplied by AUS Precision run firmware that supports
AP-DOS and also pre-defined GPIO pins for various functions. Other vendors
might be using different GPIO mappings for different hardware functions.

1.4 At the time of this article, the firmware supplied by AUS Precision does
not allow for custom GPIO pin mappings, therefore some AP-DOS functions might
run with problems due to the pin mappings being different. If this is the case
the firmware would need to be manually re-compiled with the correct mappings.
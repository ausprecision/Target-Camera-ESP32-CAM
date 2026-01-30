#include "ESPTelnet.h"
#include "FS.h"
#include "SPIFFS.h"
#include "printHelpers.h"
#include <string_asukiaaa.h>


void tel_ok(ESPTelnet &telnet)                     { telnet.println(""); telnet.println("OK");    telnet.println(""); telnet.print("C:\\>"); }
void tel_ok_msg(ESPTelnet &telnet, String message) { telnet.println(""); telnet.println(message); telnet.println(""); telnet.print("C:\\>"); }
void tel_fail(ESPTelnet &telnet, String message)   { telnet.println(""); telnet.println(message); telnet.println(""); telnet.print("C:\\>"); }

void cmd_ver(ESPTelnet &telnet) {
  telnet.println(""); 
  telnet.println("AP-DOS Version 1.00"); 
  telnet.println(""); 
  telnet.println(""); 
  telnet.print("C:\\>");
}

void print_dir_header(ESPTelnet &telnet) {
  telnet.println("");
  telnet.println(" Volume in drive C is AP-DOS_1");
  telnet.println(" Volume Serial Number is ESP32-SPIFFS"); 
  telnet.println(" Directory of C:\\");
  telnet.println("");
}

void cmd_vol(ESPTelnet &telnet) {
  telnet.println("");
  telnet.println(" Volume in drive C is AP-DOS_1");
  telnet.println(" Volume Serial Number is ESP32-SPIFFS"); 
  telnet.println("");
  telnet.print("C:\\>");
}

void cmd_help(ESPTelnet &telnet) {
  telnet.println("");
  telnet.println("For more information on a specific command, type HELP command-name");
  telnet.println("");
  telnet.println("DEL            Deletes one or more files.");
  telnet.println("DIR            Displays a list of files and subdirectories in a directory.");     // Implemented
  telnet.println("EXIT           Quits the CMD.EXE program (command interpreter).");                // Implemented
  telnet.println("HELP           Provides Help information for AP-DOS commands.");                  // Implemented
  telnet.println("TYPE           Displays the contents of a text file.");
  telnet.println("VER            Displays the AP-DOS version.");                                    // Implemented
  telnet.println("VOL            Displays a disk volume label and serial number.");                 // Implemented
  telnet.println("");
  telnet.println("CAMERA         Sets or gets camera configuration options.");
  telnet.println("CPU            Sets or gets cpu configuration options.");                         // Implemented
  telnet.println("WIFI           Sets or gets wireless configuration options.");                    // Implemented
  telnet.println("REBOOT         Reboots the system.");
  telnet.println("");
  telnet.println("For more information on tools see the command-line reference in the manual.");
  telnet.println("");
  telnet.print("C:\\>");
}

void cmd_dir(ESPTelnet &telnet) {
  telnet.println("");
  telnet.println(" Volume in drive C is AP-DOS_1");
  telnet.println(" Volume Serial Number is ESP32-SPIFFS"); 
  telnet.println(" Directory of C:\\");
  telnet.println("");
  File root = SPIFFS.open("/");
  if(root.isDirectory())
  {
    File file = root.openNextFile();
    int fileCount = 0;
    int fileTotalBytes = 0;
    while(file)
    {
      fileCount = fileCount + 1;
      fileTotalBytes = fileTotalBytes + file.size();
      //Sort out DELIMITER      
      int delimiterDot = String(file.name()).indexOf('.');
      String tempStr = String(file.name()).substring(0, delimiterDot);
      //Print FILENAME
      String stringFileName = string_asukiaaa::padEnd(String(tempStr), 9, ' ');
      stringFileName.toUpperCase();
      //Print EXTENSION
      String stringExtension = String(file.name()).substring(delimiterDot+1, delimiterDot + 4);
      stringExtension.toUpperCase();
      //Print FILE SIZE and DATE TIME
      telnet.print(stringFileName); telnet.print(stringExtension); telnet.print("       ");
      int fsize = file.size();
      
      telnet.print(string_asukiaaa::padStart(String(fsize), 7, ' '));
      telnet.println(" 08-21-24   4:11p");
      //Process Next FILE      
      file = root.openNextFile();
    }
    //Print TOTAL FILES and TOTAL BYTES and BYTES FREE
    telnet.print(string_asukiaaa::padStart(String(fileCount), 9, ' ') + " file(s)");
    telnet.println(string_asukiaaa::padStart(String(fileTotalBytes), 15, ' ') + " bytes");
    telnet.println(string_asukiaaa::padStart(String(SPIFFS.totalBytes() - SPIFFS.usedBytes()), 32, ' ') + " bytes free");

    telnet.println("");
    telnet.print("C:\\>");
  }
}

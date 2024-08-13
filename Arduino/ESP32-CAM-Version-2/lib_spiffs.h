#include "FS.h"
#include "SPIFFS.h"

#define FORMAT_SPIFFS_IF_FAILED true

void spiffs_init(){
  if (!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED)) { Serial.println("spiffs_error: SPIFFS Mount Failed, Formatting SPIFFS"); return; }
  File file;
  // Check for System Configuration Files
  if (!SPIFFS.exists("cfg-res.ini"))  {  Serial.println("spiffs_files: /cfg-res.ini does not exist, creating file");  file = SPIFFS.open("/cfg-res.ini", FILE_WRITE);  file.println("UXGA"); file.close(); }
  if (!SPIFFS.exists("cfg-xclk.ini")) {  Serial.println("spiffs_files: /cfg-xclk.ini does not exist, creating file"); file = SPIFFS.open("/cfg-xclk.ini", FILE_WRITE); file.println("10"); file.close(); }
  if (!SPIFFS.exists("cfg-aec2.ini")) {  Serial.println("spiffs_files: /cfg-aec2.ini does not exist, creating file"); file = SPIFFS.open("/cfg-aec2.ini", FILE_WRITE); file.println("1"); file.close(); }

  if (!SPIFFS.exists("wifissid.ini")) { Serial.println("spiffs_files: /wifissid.ini does not exist, creating file"); file = SPIFFS.open("/wifissid.ini", FILE_WRITE); file.println("apcamera"); file.close(); }
  if (!SPIFFS.exists("wifipass.ini")) { Serial.println("spiffs_files: /wifipass.ini does not exist, creating file"); file = SPIFFS.open("/wifipass.ini", FILE_WRITE); file.println("tspass2000"); file.close(); }
}

String readConfig(String configFile){
  File file;
  file = SPIFFS.open(configFile, FILE_READ); 
  String szReadLine = file.readStringUntil('\n');
  Serial.print("spiffs_files: read of ");
  Serial.print(configFile);
  Serial.print(" = ");
  Serial.println(szReadLine);
  file.close(); 
}
                                                              
void listDir(fs::FS &fs, const char *dirname, uint8_t levels) {
  Serial.printf("Listing directory: %s\r\n", dirname);

  File root = fs.open(dirname);
  if (!root) {
    Serial.println("- failed to open directory");
    return;
  }
  if (!root.isDirectory()) {
    Serial.println(" - not a directory");
    return;
  }

  File file = root.openNextFile();
  while (file) {
    if (file.isDirectory()) {
      Serial.print("  DIR : ");
      Serial.println(file.name());
      if (levels) {
        listDir(fs, file.path(), levels - 1);
      }
    } else {
      Serial.print("  FILE: ");
      Serial.print(file.name());
      Serial.print("\tSIZE: ");
      Serial.println(file.size());
    }
    file = root.openNextFile();
  }
}

void readFile(fs::FS &fs, const char *path) {
  Serial.printf("Reading file: %s\r\n", path);

  File file = fs.open(path);
  if (!file || file.isDirectory()) {
    Serial.println("- failed to open file for reading");
    return;
  }

  Serial.println("- read from file:");
  while (file.available()) {
    Serial.write(file.read());
  }
  file.close();
}

void writeFile(fs::FS &fs, const char *path, const char *message) {
  Serial.printf("Writing file: %s\r\n", path);

  File file = fs.open(path, FILE_WRITE);
  if (!file) {
    Serial.println("- failed to open file for writing");
    return;
  }
  if (file.print(message)) {
    Serial.println("- file written");
  } else {
    Serial.println("- write failed");
  }
  file.close();
}

void appendFile(fs::FS &fs, const char *path, const char *message) {
  Serial.printf("Appending to file: %s\r\n", path);

  File file = fs.open(path, FILE_APPEND);
  if (!file) {
    Serial.println("- failed to open file for appending");
    return;
  }
  if (file.print(message)) {
    Serial.println("- message appended");
  } else {
    Serial.println("- append failed");
  }
  file.close();
}

void renameFile(fs::FS &fs, const char *path1, const char *path2) {
  Serial.printf("Renaming file %s to %s\r\n", path1, path2);
  if (fs.rename(path1, path2)) {
    Serial.println("- file renamed");
  } else {
    Serial.println("- rename failed");
  }
}

void deleteFile(fs::FS &fs, const char *path) {
  Serial.printf("Deleting file: %s\r\n", path);
  if (fs.remove(path)) {
    Serial.println("- file deleted");
  } else {
    Serial.println("- delete failed");
  }
}

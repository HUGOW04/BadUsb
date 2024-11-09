#include <Arduino.h>
#include <Keyboard.h>
#include <SPI.h>
#include <SD.h>

const int SD_CS_PIN = 4;
String fileName = "script.txt";
File file;

void readSD();
void executeCommand(String line);
void readFromRAM();

void executeCommand(String line) {
  int space = line.indexOf(' ');
  String command = space == -1 ? line : line.substring(0, space);
  String arg = space == -1 ? "" : line.substring(space + 1);
  
  if (command == "STRING") {
      for(size_t i = 0; i < arg.length(); i++) {
          char c = arg[i];
          switch(c) {
              case ':':
                  Keyboard.press(KEY_LEFT_SHIFT);
                  Keyboard.write('.');  // : on Swedish keyboard
                  Keyboard.releaseAll();
                  break;
              case '/':
                  Keyboard.press(KEY_LEFT_SHIFT);
                  Keyboard.write('7');  // / on Swedish keyboard
                  Keyboard.releaseAll();
                  break;
              case '?':
                  Keyboard.press(KEY_LEFT_SHIFT);
                  Keyboard.write('-');  // ? on Swedish keyboard
                  Keyboard.releaseAll();
                  break;
              case '=':
                  Keyboard.press(KEY_LEFT_SHIFT);
                  Keyboard.write('0');  // = on Swedish keyboard
                  Keyboard.releaseAll();
                  break;
              case '_':
                  Keyboard.press(KEY_LEFT_SHIFT);
                  Keyboard.write('-');
                  Keyboard.releaseAll();
                  break;
              default:
                  Keyboard.write(c);
          }
      }
  }
  else if (command == "DELAY") {
    delay(arg.toInt());
  }
  else if (command == "ENTER") {
    Keyboard.write(KEY_RETURN);
  }
  else if (command == "GUI") {
    if (arg == "r") {
      Keyboard.press(KEY_LEFT_GUI);
      Keyboard.press('r');
      Keyboard.releaseAll();
    }
  }
  else if (command == "TAB") {
    Keyboard.write(KEY_TAB);
  }
  else if (command == "SPACE") {
    Keyboard.write(' ');
  }
  else if (command == "ALT") {
    Keyboard.press(KEY_LEFT_ALT);
  }
  else if (command == "SHIFT") {
    Keyboard.press(KEY_LEFT_SHIFT);
  }
  else if (command == "CTRL") {
    Keyboard.press(KEY_LEFT_CTRL);
  }
  else if (command == "REM") {
    // Comment - do nothing
  }
  // Function keys
  else if (command == "F1") {
      Keyboard.write(KEY_F1);
  }
  else if (command == "F2") {
      Keyboard.write(KEY_F2);
  }
  else if (command == "F3") {
      Keyboard.write(KEY_F3);
  }
  else if (command == "F4") {
      Keyboard.write(KEY_F4);
  }
  else if (command == "F5") {
      Keyboard.write(KEY_F5);
  }
  else if (command == "F6") {
      Keyboard.write(KEY_F6);
  }
  else if (command == "F7") {
      Keyboard.write(KEY_F7);
  }
  else if (command == "F8") {
      Keyboard.write(KEY_F8);
  }
  else if (command == "F9") {
      Keyboard.write(KEY_F9);
  }
  else if (command == "F10") {
      Keyboard.write(KEY_F10);
  }
  else if (command == "F12") {
      Keyboard.write(KEY_F12);
  }
}

void readSD() {
  Serial.println("SD card initialized!");

  file = SD.open(fileName);
  if(!file) {
    Serial.println("Failed to open " + fileName);
    return;
  }
  Serial.println("File opened successfully!");

  const int BUFFER_SIZE = 64;
  char buffer[BUFFER_SIZE];
  String line = "";
  int bytesRead = 0;
  while ((bytesRead = file.read(buffer,BUFFER_SIZE)) > 0) {
    for(int i = 0; i < bytesRead; i++)
    {
      if(buffer[i] == '\n')
      {
        Serial.println(line);
        executeCommand(line);
        line = "";
      }
      else if(buffer[i] != '\r')  // Ignore carriage returns
      {
        line += buffer[i];
      }
    }
  }

  if(line.length() >  0)
  {
      Serial.println(line);
      executeCommand(line);
  }


  file.close();
  Serial.println("\nFinished!");
}

void readFromRAM()
{
    executeCommand("ALT");
    executeCommand("F2");
    Keyboard.releaseAll();  
    executeCommand("DELAY 1000");     // Wait for dialog to fully open
    executeCommand("STRING https://www.youtube.com/watch?v=b4XpMTUlorc");
    executeCommand("DELAY 1000");      // Small delay before Enter
    executeCommand("ENTER");
}

void setup() {
  Serial.begin(115200);
  Serial.println("Starting...");
  
  SPI.begin();
  Keyboard.begin();

  if (SD.begin(SD_CS_PIN))
  {
    readSD();
  }
  else
  {
    readFromRAM();
  }
}

void loop() {

}

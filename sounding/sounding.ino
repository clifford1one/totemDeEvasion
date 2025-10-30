#include "Arduino.h"
#include "DFRobotDFPlayerMini.h"

#if (defined(ARDUINO_AVR_UNO) || defined(ESP8266))
#include <SoftwareSerial.h>
SoftwareSerial softSerial(10, 11);
#define FPSerial softSerial
#else
#define FPSerial Serial1
#endif

DFRobotDFPlayerMini myDFPlayer;

void setup()
{
#if (defined ESP32)
  FPSerial.begin(9600, SERIAL_8N1, D3, D2);
#else
  FPSerial.begin(9600);
#endif

  Serial.begin(115200);

  if (!myDFPlayer.begin(FPSerial, true, true)) {
    Serial.println("Error: Revisar conexion y tarjeta SD");
    while(true) delay(0);
  }
  
  myDFPlayer.volume(30);
  myDFPlayer.play(1,1);
}

void loop()
{
  static unsigned long timer = millis();
  
  if (millis() - timer > 3000) {
    timer = millis();
    myDFPlayer.next();
  }
}
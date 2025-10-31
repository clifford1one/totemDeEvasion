#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN 9
#define SS_PIN 10
int togglePin = 2;
int pinRed = 3;
int pinGreen = 4;

MFRC522 mfrc522(SS_PIN, RST_PIN);

//audio
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

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  pinMode(togglePin, INPUT);
  pinMode(pinRed, OUTPUT);
  pinMode(pinGreen, OUTPUT);


  //audio

#if (defined ESP32)
  FPSerial.begin(9600, SERIAL_8N1, D3, D2);
#else
  FPSerial.begin(9600);
#endif

  Serial.begin(115200);

  if (!myDFPlayer.begin(FPSerial, true, true)) {
    Serial.println("Error: Revisar conexion y tarjeta SD");
    while (true) delay(0);
  }
}

void loop() {
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    int state = digitalRead(togglePin);

    if (state == HIGH) {
      Serial.println("Gracias, buen viaje!");
      myDFPlayer.play(1);

      digitalWrite(pinGreen, HIGH);
      delay(1800);
      digitalWrite(pinGreen, LOW);


    } else {
      Serial.println("Saldo insuficiente");
      myDFPlayer.play(2);

      digitalWrite(pinRed, HIGH);
      delay(1800);
      digitalWrite(pinRed, LOW);
    }

    mfrc522.PICC_HaltA();
  }
}

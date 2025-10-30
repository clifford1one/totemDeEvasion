#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN 9
#define SS_PIN 10
int togglePin=2;
int pinRed=3;
int pinGreen=4;

MFRC522 mfrc522(SS_PIN, RST_PIN);

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  pinMode(togglePin, INPUT);
  pinMode(pinRed, OUTPUT);
  pinMode(pinGreen, OUTPUT);
  
}

void loop() {
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    int state = digitalRead(togglePin);

    if (state == HIGH) {
      Serial.println("Gracias, buen viaje!");
      digitalWrite(pinGreen, HIGH);
      delay(1800);
      digitalWrite(pinGreen, LOW);

    } else {
      Serial.println("Saldo insuficiente");
      digitalWrite(pinRed, HIGH);
      delay(1800);
      digitalWrite(pinRed, LOW);

    }

    mfrc522.PICC_HaltA();
  }
}

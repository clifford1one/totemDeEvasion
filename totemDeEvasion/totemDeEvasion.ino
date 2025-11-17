/////////////////////////////////////////////////////////////
/*este es un código hecho por santiagoClifford para el 
proyecto totemDeEvasion. Este codigo controla un dispositivo
que recrea la experiencia de pagar la micro en la vida real.

Este dispositivo reacciona frente a nfc, y a traves de
un parlante y varios led, le informa al usuarie si tiene
saldo o no. Esto dependerá del estado de un interruptor.*/
/////////////////////////////////////////////////////////////


#include "Arduino.h"
#include "DFRobotDFPlayerMini.h"
//audio
#include <SPI.h>
#include <MFRC522.h>

//variables audio
const int paseAdelante = 1;
const int saldoInsuficiente = 2;
const int delayLed = 1800;

//pines lector nfc
const int resetPin = 9;
const int ssPin = 10;

//otros pines
int pinToggle = 2;
int pinLedRojo = 3;
int pinLedVerde = 4;

//intancia del lector nfc
MFRC522 mfrc522(ssPin, resetPin);

//configuracion de comunicación serial del modulo mp3
#if (defined(ARDUINO_AVR_UNO) || defined(ESP8266))
#include <SoftwareSerial.h>
SoftwareSerial softSerial(10, 11);
#define FPSerial softSerial
#else
#define FPSerial Serial1
#endif

//intancia mp3
DFRobotDFPlayerMini myDFPlayer;

void setup() {
  Serial.begin(115200);

  //inicialiazación lector nfc
  SPI.begin();
  mfrc522.PCD_Init();

  //config pines
  pinMode(pinToggle, INPUT);
  pinMode(pinLedRojo, OUTPUT);
  pinMode(pinLedVerde, OUTPUT);

  //inicialización audio
  FPSerial.begin(9600);


  if (!myDFPlayer.begin(FPSerial, true, true)) {
    Serial.println("Error: Revisar conexion y tarjeta SD");
    while (true) delay(0);
  }
}

void loop() {
  //aqui se setea el lector nfc para que este verifique si se detecta un nfc
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {

    //leer el estado del toggle(interruptor)
    int state = digitalRead(pinToggle);

    //esta es la condición de con saldo/sin saldo
    if (state == HIGH) {
      //SI TIENES SALDO
      myDFPlayer.play(paseAdelante);
      digitalWrite(pinLedVerde, HIGH);
      Serial.println("ADELANTE PASE");
      delay(delayLed);
      digitalWrite(pinLedVerde, LOW);


    } else {
      //SI NO TIENES SALDO
      myDFPlayer.play(saldoInsuficiente);
      digitalWrite(pinLedRojo, HIGH);
      Serial.println("Saldo insuficiente");
      delay(delayLed);
      digitalWrite(pinLedRojo, LOW);
    }
    //detener com lector nfc
    mfrc522.PICC_HaltA();
  }
}

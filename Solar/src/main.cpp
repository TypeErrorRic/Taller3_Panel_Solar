#include <Arduino.h>
#include <CapturaDatos.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#define LIMITE_ERROR 10
#define PIN_CARGA 11

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(9600);
  //Cofniguración de rele:
  pinMode(PIN_CARGA,OUTPUT);
  //Configuración de Control y Adquisicón de datos:
  setupCapture();
  //Configuración de LCD
}

void loop() {
  //Cargar Batería:
  if((error < LIMITE_ERROR) && (error > -LIMITE_ERROR)) digitalWrite(PIN_CARGA,HIGH);
  else digitalWrite(PIN_CARGA,LOW);
  //Determinar voltaje minimo aceptable:
  if(capturaVoltajePanel() < voltajeUmbral) caso = 0;
  else caso = 1;
  //Imprimir valores por pantalla.
  Serial.println(error);
  _delay_ms(1000);
}
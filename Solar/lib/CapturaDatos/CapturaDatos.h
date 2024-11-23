#ifndef CAPTURA_DATOS_H
#define CAPTURA_DATOS_H

#include <Control.h>

//Configuración de Captura de voltaje de Batería y Control:
#define PINPWM (unsigned short) 3
#define PIN_VOLTAJE_BATERIA (unsigned short) A0
#define PERIODO_MUESTREO 0.05 // 50 ms.
#define VALOR_REFERENCIA 9.4

volatile extern unsigned short caso;

//Configuración del Sensor de Corriente:
#define sensorPin  A2 // Pin analógico donde se conecta el ACS712
#define sensitivity 0.185 // Sensibilidad del ACS712 (V/A), para 5A es 185 mV/A
#define VREF 5.0 // Voltaje de referencia del Arduino

//Configuración del Sensor de Voltaje:
#define sensor2Pin A1
#define voltajeUmbral 2

void setupCapture();
float capturaCorrientePanel();
float capturaVoltajePanel();

#endif
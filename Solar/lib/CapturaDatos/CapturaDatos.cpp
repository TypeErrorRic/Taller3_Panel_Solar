#include <Arduino.h>
#include <CapturaDatos.h>

volatile unsigned short caso = 0;

//Control: Kp, Ki, Kd:
Control control(0.2,4,0.04,PERIODO_MUESTREO,VALOR_REFERENCIA);

//Variables:
float CorrientePanel = 0;
float VoltajePanel = 0;

void setupCapture()
{
  //Configura el temporizador a 50 ms:
  cli(); // Desactivar interrupciones mientras configuramos
  TCCR1A = 0; // Modo normal de operación
  TCCR1B = 0; // Limpiar registros para configurar
  // Configurar el preescaler (prescaler)
  TCCR1B |= (1 << WGM12);  // Modo CTC (Clear Timer on Compare Match)
  TCCR1B |= (1 << CS12) | (1 << CS10); // Prescaler de 1024
  // Calcular el valor de comparación para el temporizador
  OCR1A = 782;
  // Habilitar interrupción de comparación para OCR1A
  TIMSK1 |= (1 << OCIE1A);
  sei(); // Activar interrupciones

  //Frecuencia: 4khz
  TCCR2B &= ~((1 << CS22) | (1 << CS21) | (1 << CS20));
  TCCR2B |= (1 << CS20);
  
  //Configuración de Pines:
  pinMode(PINPWM,OUTPUT);
  analogWrite(PINPWM,0);
}

//Acción de control:
ISR(TIMER1_COMPA_vect)
{
  switch (caso)
  {
  //Control
  case 1:
    analogWrite(PINPWM,control.control((unsigned short)(((float)analogRead(PIN_VOLTAJE_BATERIA)/Vmax)*255)));
    break;
  //Voltaje umbral minimo de amplificacion:
  default:
    analogWrite(PINPWM,0);
    error = 0;
    break;
  }    
}


//Corriente Panel:
float capturaCorrientePanel()
{
  int sensorValue = analogRead(sensorPin);
  // Convierte el valor analógico a voltaje
  float voltage = (sensorValue * VREF) / ((float)Vmax);
  // Calcula la corriente en amperios
  return CorrientePanel = (voltage - (VREF / 2)) / sensitivity; // El ACS712 tiene un punto de salida a 2.5V cuando la corriente es 0A
}

//Voltaje Panel:
float capturaVoltajePanel()
{
  return ((float)analogRead(sensor2Pin)*(VREF/Vmax))*1.8;
}
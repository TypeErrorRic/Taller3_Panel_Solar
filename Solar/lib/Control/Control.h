#ifndef CONTROL_H
#define CONTROL_H

#define Vmax 1023
#define VoltajeSalida 19.74

volatile extern int error;
extern unsigned short PWMvalor;

class Control
{
private:
    float Kp;
    float Ki;
    float Kd;
    float dt;
    float integral;
    float derivada;
    int errorP;
    float ref;
public:
    Control(float, float, float, float, float);
    float AccionIntegral(int);
    float AccionProporcional(int);
    float AccionDerivativa(int);
    unsigned short control(unsigned short);
};

#endif
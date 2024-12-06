#include <Control.h>

volatile int error = 0;
unsigned short PWMvalor = 0;

Control::Control(float Kp, float Ki, float Kd, float dt, float ref) : Kp(Kp), Ki(Ki), Kd(Kd), dt(dt)
{
    this->ref = static_cast<int>((ref * 255) / VoltajeSalida);
    this->derivada = 0;
    this->errorP = 0;
    this->integral = 0;
}

static int constrain(int value, int min, int max)
{
    if (value < min)
    {
        return min; // Devuelve el límite inferior
    }
    else if (value > max)
    {
        return max; // Devuelve el límite superior
    }
    return value; // Devuelve el valor sin cambios
}

float Control::AccionIntegral(int error)
{
    // Acumular el error
    this->integral += error * dt;
    // Limitar la parte integral para evitar windup
    this->integral = constrain(this->integral, -ValorPWMMax / this->Ki, ValorPWMMax / this->Ki);
    return this->Ki * this->integral; // Devuelve el término integral calculado
}

float Control::AccionProporcional(int error)
{
    return this->Kp * error;
}

float Control::AccionDerivativa(int error)
{
    this->derivada = (error - this->errorP)/this->dt;
    this->derivada = constrain(this->derivada, -ValorPWMMax, ValorPWMMax);
    return this->derivada * this->Kd;
}

unsigned short Control::control(unsigned short valorCapturado)
{
    error = ref - valorCapturado;
    // Obtener la acción Proporcional:
    float proportional = AccionProporcional(error);
    //Evitar subidas bruscas de Voltaje.
    if (valorCapturado > 230) integral = 0;
    // Obtener la acción integral
    float integralAction = AccionIntegral(error);
    // Obtener la acción derivativa
    float derivadaAction = 0;
    if(((error > -22 && error < 22) && ((error < -10 || error > 10)))
        && (errorP > -25 && errorP < 25) && (PWMvalor > 15 && PWMvalor < ValorPWMMax))
    {
        derivadaAction = AccionDerivativa(error);
        if (error < 0)
            this->integral -= derivadaAction;
        else
            this->integral += derivadaAction;
    }
    //Actualizar error anterior:
    errorP = error;
    // Salida total
    PWMvalor = constrain(proportional + integralAction + derivadaAction, 0, ValorPWMMax);
    return PWMvalor;
}

void Control::setIntegral(float valor)
{
    this->integral = valor;
}
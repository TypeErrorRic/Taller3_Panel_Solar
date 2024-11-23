#include <Control.h>

volatile int error = 0;
unsigned short PWMvalor = 0;

Control::Control(float Kp, float Ki, float Kd, float dt, float ref) : Kp(Kp), Ki(Ki), Kd(Kd), dt(dt)
{
    Control::ref = static_cast<int>((ref * 255) / VoltajeSalida);
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
    integral += error * dt;
    // Limitar la parte integral para evitar windup
    integral = constrain(integral, -230.0 / Ki, 230.0 / Ki);
    return Ki * integral; // Devuelve el término integral calculado
}

float Control::AccionProporcional(int error)
{
    return Kp * error;
}

float Control::AccionDerivativa(int error)
{
    derivada = (error - errorP)/dt;
    derivada = constrain(derivada, -230.0, 230.0);
    return derivada * Kd;
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
        && (errorP > -25 && errorP < 25) && (PWMvalor > 15 && PWMvalor < 230))
    {
        derivadaAction = AccionDerivativa(error);
        if (error < 0)
            integral -= derivadaAction;
        else
            integral += derivadaAction;
    }
    //Actualizar error anterior:
    errorP = error;
    // Salida total
    PWMvalor = constrain(proportional + integralAction + derivadaAction, 0, 230);
    return PWMvalor;
}
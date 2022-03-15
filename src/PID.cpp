#include "PID.h"

PID::PID()
{
    Kp = 0;
    Ki = 0;
    Kd = 0;
    error = 0;
    oldError = 0;
    errorIntegral = 0;
    errorDerivative = 0;
    delayTime = 0;
    control = 0;
    zeroInt_DerFlag = false;
    min = 0;
    max = 0;
}

PID::PID(float proportionalK, float integralK, float derivativeK, int minVal, int maxVal, int delay)
{
    Kp = proportionalK;
    Ki = integralK;
    Kd = derivativeK;
    error = 0;
    oldError = 0;
    errorIntegral = 0;
    errorDerivative = 0;
    delayTime = delay;
    control = 0;
    zeroInt_DerFlag = false;
    min = minVal;
    max = maxVal;
}

PID::~PID()
{}

void PID::calcControl(float desired, float currentVal)
{
    //e(t)
    error = desired - currentVal; 

    errorIntegral = errorIntegral + delayTime*error/1000.0;

    errorDerivative = 1000.0*(error-oldError)/delayTime;

    if(zeroInt_DerFlag)
    {
        errorIntegral = 0;
        errorDerivative = 0;
    }

    //u(t)
    control = Kp*error + Ki*errorIntegral + Kd*errorDerivative;
    saturateControl();

    oldError = error;
    zeroInt_DerFlag = false;
}

void PID::setDelayTime(int delayMilliseconds)
{
    delayTime = delayMilliseconds;
}

void PID::saturateControl()
{
   if(control > max)
    control = max;
  else if(control < min)
    control = min;
}

float PID::getControl()
{
    return control;
}

float PID::getError(){
    return error;
}

float PID::getOldError(){
    return oldError;
}

float PID::getErrorIntegral(){
    return errorIntegral;
}

float PID::getErrorDerivative(){
    return errorDerivative;
}

void PID::setErrorInt_DerToZero()
{
    zeroInt_DerFlag = true;
}
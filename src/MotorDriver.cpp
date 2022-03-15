#include "Arduino.h"
#include "MotorDriver.h"



MotorDriver::MotorDriver()
{
    input1 = 0;
    input2 = 0;
    enableMotor = 0;
}

MotorDriver::~MotorDriver()
{}

MotorDriver::MotorDriver(int in1, int in2, int en)
{
    input1 = in1;
    input2 = in2;
    enableMotor = en;
}

void MotorDriver::mdInit()
{
    pinMode(enableMotor, OUTPUT);
    pinMode(input1, OUTPUT);
    pinMode(input2, OUTPUT);
}


void MotorDriver::setMotorForward(){
  digitalWrite(input1, LOW);
  digitalWrite(input2, HIGH);
}

void MotorDriver::setMotorReverse(){
  digitalWrite(input1, HIGH);
  digitalWrite(input2, LOW);
}

void MotorDriver::applyControlInput(float controlInput){
  if( controlInput < 0)
    setMotorReverse();
  else if( controlInput> 0)
    setMotorForward();

  analogWrite(enableMotor, fabs(controlInput));
}
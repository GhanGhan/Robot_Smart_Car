#include <Arduino.h>
#include "SerialInputParsing.h"

int getNegativeSpeed_Port(int num){
  byte digits = num - 2;
  int motorSpeed;
  if(digits == 1){//only one digit was entered
    int value1 = Serial.read();
    motorSpeed = -(value1 - '0');
  }
  else if(digits == 2){//two digits were entered
    int value1 = Serial.read(), value2 = Serial.read();
    motorSpeed = -(10*(value1 - '0') + value2 - '0');
  }
  else if(digits == 3){//three digits were entered
    int value1 = Serial.read(), value2 = Serial.read(), value3 = Serial.read();
    motorSpeed = -(100*(value1 - '0') + 10*(value2 -'0') +  value3 - '0');
  }
  return motorSpeed;
}

int getPositiveSpeed_Port(int num, int recievedVal){
  byte digits = num - 1;
  int motorSpeed;
    if(digits == 1){//only one digit was entered
    motorSpeed = recievedVal - '0';
  }
  else if(digits == 2){//two digits were entered
    int value = Serial.read();
    motorSpeed = 10*(recievedVal - '0') + value - '0';
  }
  else if(digits == 3){//three digits were entered
    int value2 = Serial.read(), value3 = Serial.read();
    motorSpeed = 100*(recievedVal - '0') + 10*(value2 -'0') +  value3- '0';
  }
  return motorSpeed;
}

int getSpeed_Port(int num, int recievedVal){
  int motorSpeed;
    if(recievedVal == '0') // Set motor speed to 0
    {
      motorSpeed = 0;
    }
    else if(recievedVal == '-')//A non-zero negative motor speed was entered
    {
      motorSpeed = getNegativeSpeed_Port(num);
    }
    else if (recievedVal >= '1' && recievedVal <= '9')//A non-zero motor positive speed was entered
    {
      motorSpeed = getPositiveSpeed_Port(num, recievedVal);
    }//end recievedVal if-else-if statements
    return motorSpeed;
}

int getNegativeRot_Port(int num){
  byte digits = num - 2;
  int motorRot;
  if(digits == 1){//only one digit was entered
    int value1 = Serial.read();
    motorRot = -(value1 - '0');
  }
  else if(digits == 2){//two digits were entered
    int value1 = Serial.read(), value2 = Serial.read();
    motorRot = -(10*(value1 - '0') + value2 - '0');
  }
  else if(digits == 3){//three digits were entered
    int value1 = Serial.read(), value2 = Serial.read(), value3 = Serial.read();
    motorRot = -(100*(value1 - '0') + 10*(value2 -'0') +  value3 - '0');
  }
  return motorRot;
}

int getPositiveRot_Port(int num, int recievedVal){
  byte digits = num - 1;
  int motorRot;
  if(digits == 1){//only one digit was entered
    motorRot = recievedVal - '0';
  }
  else if(digits == 2){//two digits were entered
    int value = Serial.read();
    motorRot = 10*(recievedVal - '0') + value - '0';
  }
  else if(digits == 3){//three digits were entered
    int value2 = Serial.read(), value3 = Serial.read();
    motorRot = 100*(recievedVal - '0') + 10*(value2 -'0') +  value3- '0';
  }
  return motorRot;
}

int getRot_Port(int num, int recievedVal){
  int motorRot;
  recievedVal = Serial.read();
  num = num - 1;
  if(recievedVal == '0') // Set motor speed to 0
  {
    motorRot = 0;
  }
  else if(recievedVal == '-')//A non-zero negative motor speed was entered
  {
    motorRot = getNegativeRot_Port(num);
  }
  else if (recievedVal >= '1' && recievedVal <= '9')//A non-zero motor positive speed was entered
  {
    motorRot = getPositiveRot_Port(num, recievedVal);
  }//end recievedVal if-else-if statements
  return motorRot;
}


int getNegativeSpeed_BT(int num){
  byte digits = num - 2;
  int speed;
  if(digits == 1){//only one digit was entered
    int value1 = Serial1.read();
    speed = -(value1 - '0');
  }
  else if(digits == 2){//two digits were entered
    int value1 = Serial1.read(), value2 = Serial1.read();
    speed = -(10*(value1 - '0') + value2 - '0');
  }
  else if(digits == 3){//three digits were entered
    int value1 = Serial1.read(), value2 = Serial1.read(), value3 = Serial1.read();
    speed = -(100*(value1 - '0') + 10*(value2 -'0') +  value3 - '0');
  }
  return speed;
}

int getPositiveSpeed_BT(int num, int recievedVal){
  byte digits = num - 1;
  int speed;
    if(digits == 1){//only one digit was entered
    speed = recievedVal - '0';
  }
  else if(digits == 2){//two digits were entered
    int value = Serial1.read();
    speed = 10*(recievedVal - '0') + value - '0';
  }
  else if(digits == 3){//three digits were entered
    int value2 = Serial1.read(), value3 = Serial1.read();
    speed = 100*(recievedVal - '0') + 10*(value2 -'0') +  value3- '0';
  }
  return speed;
}

int getSpeed_BT(int num1){
  int motorSpeed;
  int recievedVal = Serial1.read();
  num1 = num1 - 2; //'-2' for the START_BYTE_SPEED and END_BYTE_SPEED bytes
  if(recievedVal == '0') // Set motor speed to 0
  {
    motorSpeed = 0;
  }
  else if(recievedVal == '-') //A non-zero negative motor speed was recieved
  {
    motorSpeed = getNegativeSpeed_BT(num1);
  }
  else if (recievedVal >= '1' && recievedVal <= '9') //A non-zero motor positive speed was recieved
  {
    motorSpeed = getPositiveSpeed_BT(num1, recievedVal);
  }
  return motorSpeed;
}


int getNegativeRot_BT(int num){
  byte digits = num - 2;
  int motorRot;
  if(digits == 1){//only one digit was entered
    int value1 = Serial1.read();
    motorRot = -(value1 - '0');
  }
  else if(digits == 2){//two digits were entered
    int value1 = Serial1.read(), value2 = Serial1.read();
    motorRot = -(10*(value1 - '0') + value2 - '0');
  }
  else if(digits == 3){//three digits were entered
    int value1 = Serial1.read(), value2 = Serial1.read(), value3 = Serial1.read();
    motorRot = -(100*(value1 - '0') + 10*(value2 -'0') +  value3 - '0');
  }
  return motorRot;
}

int getPositiveRot_BT(int num, int recievedVal){
  byte digits = num - 1;
  int motorRot;
    if(digits == 1){//only one digit was entered
    motorRot = recievedVal - '0';
  }
  else if(digits == 2){//two digits were entered
    int value = Serial1.read();
    motorRot = 10*(recievedVal - '0') + value - '0';
  }
  else if(digits == 3){//three digits were entered
    int value2 = Serial1.read(), value3 = Serial1.read();
    motorRot = 100*(recievedVal - '0') + 10*(value2 -'0') +  value3- '0';
  }
  return motorRot;
}

int getRot_BT(int num){
  int motorRot;
  int recievedVal = Serial1.read();
  num = num - 2;
  if(recievedVal == '0') // Set motor speed to 0
  {
    motorRot = 0;
  }
  else if(recievedVal == '-')//A non-zero negative motor speed was entered
  {
    //Serial.print("Recived minus");
    motorRot = getNegativeRot_BT(num);
  }
  else if (recievedVal >= '1' && recievedVal <= '9')//A non-zero motor positive speed was entered
  {
    //Serial.print("Recived 1-9");
    motorRot = getPositiveRot_BT(num, recievedVal);
  }//end recievedVal if-else-if statements
  return motorRot;
}


#include <Arduino.h>
#include "wheelEncoder.h"

//PWM pins for MotorA and MotorB respectively.  They control the motor speed.
const int enA = 10;//9; //Enable A
const int enB = 9;//10;//Enable B

//Input Pins for MotorA and B.  They control the direction of the motors
const int in1 = 5;//7;//Inputs for Motor A
const int in2 = 4;//6;
const int in3 = 7;//5;//Inputs for Motor B
const int in4 = 6;//4;

//Encoder pins for Motor A
const int encA1 = 2;//A Pin -> the interrupt pin 0
const int encA2 = 8;//B Pin
int previous_A1;
volatile int durationA = 0;//the number of the pulses
float rpmA = 0;
volatile bool DirectionA;//the rotation direction

//Encoder pins for Motor B
const int encB1 = 3;//A Pin -> the interrupt pin 1
const int encB2 = 11;//B Pin
int previous_B1;
volatile int durationB = 0;//the number of the pulses
float rpmB = 0;
volatile bool DirectionB;//the rotation direction

const int Pulses_Per_Rotation = 1920;

const int mills = 1000;
const int delayTime = 20; //10

int speedA = 0;
int speedB = 0;
int speed;

int rotSpeed = 0;
float actualRotSpeed = 0;



void EncoderInit()
{
  DirectionA = true;//default -> Forward
  DirectionB = true;
  pinMode(encA2,INPUT);
  pinMode(encB2, INPUT);
  attachInterrupt(digitalPinToInterrupt(encA1), wheelSpeedA, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encB1), wheelSpeedB, CHANGE);
}


void wheelSpeedA()
{
  int current_A1 = digitalRead(encA1);
  if((previous_A1 == LOW) && current_A1==HIGH)//only checking direction when A1 rises from LOW to HIGH
  {
    int val_A2 = digitalRead(encA2);
    if(val_A2 == LOW && !DirectionA)//Checks if wheel is no longer moving forward and is now in reverse
    {
      DirectionA = true;//false; //Reverse
    }
    else if (val_A2 == HIGH && DirectionA)//Checks if wheel is no longer reversing and is now moving forward
    {
      DirectionA = false;//true;  //Forward
    }
  }
  previous_A1 = current_A1;

  if(DirectionA)  durationA++;
  else  durationA--;
}

void wheelSpeedB()
{
  int current_B1 = digitalRead(encB1);
  if((previous_B1 == LOW) && current_B1==HIGH)//only checking direction when B1 rises from LOW to HIGH
  {
    int val_B2 = digitalRead(encB2);
    if(val_B2 == LOW && DirectionB)//Checks if wheel is no longer moving forward and is now in reverse
    {
      DirectionB = false;//true;
    }
    else if(val_B2 == HIGH && !DirectionB)//Checks if wheel is no longer reversing and is now moving forward
    {
      DirectionB = true;//false;
    }
  }
  previous_B1 = current_B1;
  

  if(DirectionB)  durationB++;
  else  durationB--;
}

void setMotorAReverse(){
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  //DirectionA = false;
}

void setMotorBReverse(){
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  //DirectionB = false;
}

void setMotorAForward(){
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  //DirectionA = true;
}

void setMotorBForward(){
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  //DirectionB = true;
}
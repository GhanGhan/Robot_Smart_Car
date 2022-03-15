#include <Arduino.h>
#include "PID.h"
#include "MotorDriver.h"
#include "Encoder.h"
#include "SerialInputParsing.h"

// PID CONTROL PARAMETERS
const float KP = 5.75;    // Proportional Feedback
const float KI = 7;       // Integral Feedback
const float KD = 0.02;    // Derivative feedback

const int delayTime = 20;
const int mills = 1000;
PID controlWheelA = PID(KP, KI, KD, -255, 255, delayTime);
PID controlWheelB = PID(KP, KI, KD, -255, 255, delayTime);

//MOTOR CONTROL PARAMETERS
//PWM pins for MotorA and MotorB respectively.  They control the motor speed.
const int enA = 10; //Enable A
const int enB = 9;//Enable B
//Input Pins for MotorA and B.  They control the direction of the motors
const int in1 = 5;//Inputs for Motor A
const int in2 = 4;
const int in3 = 7;//Inputs for Motor B
const int in4 = 6;
MotorDriver motorA = MotorDriver(in1, in2, enA);
MotorDriver motorB = MotorDriver(in3, in4, enB);

//ENCODER PARAMETERS
const int Pulses_Per_Rotation = 1920;
//Encoder Index
const int encoder0 = 0;
const int encoder1 = 1;
//Encoder pins for Motor A
const int enc0A = 2;//A Pin -> the interrupt pin 0
const int enc0B = 8;//B Pin
//Encoder pins for Motor B
const int enc1A = 3;//A Pin -> the interrupt pin 0
const int enc1B = 11;//B Pin
Encoder encoderWheel0 = Encoder(enc0A, enc0B, Pulses_Per_Rotation, delayTime);
Encoder encoderWheel1 = Encoder(enc1A, enc1B, Pulses_Per_Rotation, delayTime);

float speedA, speedB, speed;
float rpmA, rpmB;

float rotSpeed, actualRotSpeed;


//Error Detection bytes
const int START_BYTE_SPEED = 'a';
const int END_BYTE_SPEED = 'b';
const int START_BYTE_ROT = 'f';
const int END_BYTE_ROT = 'g';

const float R = 3.25;
const float L = 13;

const int phoneBluetooth = 52;  //checks if have signal to connect to phone via bluetooth
const int serialPort = 53;      //checks if have signal to connect to a serial port via bluetooth or USB


void printFromPort(int printErrors = 0, int printRPMs = 1, int printControlSignals = 0);
void printFromBT(int printErrors = 0, int printRPMs = 1, int printControlSignals = 0);

void applyControlInput();


void setup() {
  
  Serial.begin(115200);
  Serial1.begin(115200);
  pinMode( 18, INPUT_PULLUP );
  pinMode( 19, INPUT_PULLUP );

  motorA.mdInit();
  motorB.mdInit();
  //Communication Source
  pinMode(phoneBluetooth, INPUT);
  pinMode(serialPort, INPUT);
  // Set initial rotation direction
  motorA.setMotorForward();
  motorB.setMotorForward();
  //Initialize the Motor Encoders
  encoderWheel0.EncoderInit(encoder0, 0);
  encoderWheel1.EncoderInit(encoder1, 0);

  
  speedA = 0; speedB = 0; speed = 0;
  rpmA = 0; rpmB = 0;
  rotSpeed = 0;
  actualRotSpeed = 0;

}

void loop() {
  //Testing Ramp Input
  /*
  if(speed < 130){
    speed += 2;
    speedA = speed;
    speedB = speed;
    analogWrite(enA, speedA);
    analogWrite(enB, speedB);
  }
  */

  //Testing Sinuisoidal Input
  /*
  float step = 0.2;
  speed = 130.0*sin(step);
  speedA = speed; speedB = speed;
  step = step+0.2;
  */
  
  //////////%%%%%%%%%%% GET SPEED (cm/s) AND OMEGA (deg/second)
  ///---------------FOR SERIAL PORT CONNECTION (USB/BLUETOOTH) CONNECTION!!!!!!!
  //get speed value
  int num = Serial.available();
  int recievedVal = Serial.read();
  if(num > 0  && digitalRead(serialPort)==HIGH && recievedVal != 'c')
  { 
    speed = getSpeed_Port(num, recievedVal);
    //r(t)
    speedA = speed; speedB = speed;
    Serial.read();
  }// end=if statement -- SERIALPORT

  //get rotation speed value
  if(num > 0  && digitalRead(serialPort)==HIGH && recievedVal == 'c')
  {
    rotSpeed = getRot_Port(num, recievedVal);
    Serial.read();
  }
  
  
    
  ///---------------FOR BLUETOOTH CONNECTION!!!!!!!
  int num1 = Serial1.available();
  int recievedValBT = Serial1.read();


  if(num1 > 0 && digitalRead(phoneBluetooth)==HIGH )
  {
    if(recievedValBT == START_BYTE_SPEED)
    {
      int tempSpeed;
      tempSpeed = getSpeed_BT(num1);
      char dig = Serial1.read();
      if(dig == END_BYTE_SPEED && tempSpeed <= 130 && tempSpeed >= -130){
        speed = tempSpeed;
      }
      else{}
      Serial1.read();//to get the carriage return byte
    }// end if statement -- PHONEBLUETOOTH
    

    if(recievedValBT == START_BYTE_ROT)
    {
      int tempRot;
      tempRot = getRot_BT(num1);
      char dig = Serial1.read();
      if(dig == END_BYTE_ROT && tempRot <= 360 && tempRot >= -360){
        rotSpeed = tempRot;

      }
      else{}
      Serial1.read();//to get the carriage return byte
    }// end if statement -- PHONEBLUETOOTH
  }
  
  float v = speed*2*PI*R/60; //convert speed (rpm) to (m/s)
  float omega = rotSpeed*PI/180; //convert (deg/s) to (rad/s)

  float vl(0), vr(0);

  vr = (2*v+omega*L)/(2*R);//convert (m/s) to (rad/s)
  vl = (2*v-omega*L)/(2*R);

  speedA = 60*vl/(2*PI);// convert rads/s to RPM
  speedB = 60*vr/(2*PI);

  if(speedA == 0 && speedB == 0){
    controlWheelA.setErrorInt_DerToZero();
    controlWheelB.setErrorInt_DerToZero();
  }
  controlWheelA.calcControl(speedA, rpmA);
  controlWheelB.calcControl(speedB, rpmB);
  
  // Saturate control signal if it goes above/below the largest value for analogWrite
  applyControlInput();

  rpmA = encoderWheel0.getRPM();
  rpmB = encoderWheel1.getRPM();

  actualRotSpeed = (rpmB-rpmA)*1.5;
 
  //if(digitalRead(serialPort)==HIGH) ---Commented out because I want to be able to plot speed even when connected to Phone
  //{
    printFromPort(0, 1, 1);
  //}

  if(digitalRead(phoneBluetooth)==HIGH)
  {
    printFromBT(0, 1, 0);
  }
  

  encoderWheel0.setPulsesToZeros();
  encoderWheel1.setPulsesToZeros();

  delay(delayTime);
}// END PROGRAM LOOP

void printFromPort(int printErrors, int printRPMs, int printControlSignals){
  if(printErrors == 1){
    Serial.print("Error A: ");
    Serial.print(controlWheelA.getError());
    Serial.print(",");
    Serial.print("Error B: ");
    Serial.print(controlWheelB.getError());
    Serial.print(",");
  }
  
  if(printRPMs == 1){
    Serial.print("RpmA: ");
    Serial.print(rpmA);
    Serial.print(",");
    Serial.print(" Rpm B: ");
    Serial.print(rpmB);
    Serial.print(",");
  }
  
  if(printControlSignals == 1){
    Serial.print("Control A: ");
    Serial.print(controlWheelA.getControl());
    Serial.print(",");
    Serial.print("Control B: ");
    Serial.print(controlWheelB.getControl());
    Serial.print(",");
  }

  
  Serial.print(" Rot: ");
  Serial.print(rotSpeed);
  Serial.print(",");

  Serial.print(" ARot: ");
  Serial.print(actualRotSpeed);
  Serial.print(",");

  Serial.print(" SpeedA: ");
  Serial.print(speedA);
  Serial.print(",");

  Serial.print(" SpeedB: ");
  Serial.print(speedB);
  Serial.print(",");
  
  
  Serial.print(" Speed: ");
  Serial.print(speed);
  Serial.println();
}

void printFromBT(int printErrors, int printRPMs, int printControlSignals){
  if(printErrors == 1){
    Serial1.print("Error A: ");
    Serial1.print(controlWheelA.getError());
    Serial1.print(",");
    Serial1.print("Error B: ");
    Serial1.print(controlWheelB.getError());
    Serial1.print(",");
  }
  
  if(printRPMs == 1){
    Serial1.print("Left: ");
    Serial1.print(rpmA);
    Serial1.print(",");
    Serial1.print(" Right: ");
    Serial1.print(rpmB);
    Serial1.print(",");
  }
  
  if(printControlSignals == 1){
    Serial1.print("Control A: ");
    Serial1.print(controlWheelA.getControl());
    Serial1.print(",");
    Serial1.print("Control B: ");
    Serial1.print(controlWheelB.getControl());
    Serial1.print(",");
  }
  
  Serial1.print(" Sent Speed: ");
  Serial1.print(speed);
  Serial1.println();

  Serial1.print("Sent DGS: ");
  Serial1.print(rotSpeed);
  Serial1.print(",");

  Serial1.print(" Actual DGS: ");
  Serial1.println(actualRotSpeed);
}


void applyControlInput(){
  motorA.applyControlInput(controlWheelA.getControl());
  motorB.applyControlInput(controlWheelB.getControl());
}


#include <Arduino.h>
#include "wheelEncoder.h"

// PID Control parameters
const float KP = 5.75;    // Proportional Feedback
const float KI = 7;       // Integral Feedback
const float KD = 0.02;    // Derivative feedback

//Current and past error values i.e e(n) and e(n-1) for Motors A and B
float errorA(0), errorB(0);
float oldErrorA(0), oldErrorB(0);
//Control signal
float conA(0), conB(0);   
//"Integral" of current and past error values of Motors A and Motors B
float errorAIntegral(0), errorBIntegral(0);
//"Derivative" of current and previous error values of Motors A and Motors B
float errorADerivative(0), errorBDerivative(0);
//Error Detection bytes
const int START_BYTE = 'a';
const int END_BYTE = 'b';

const int phoneBluetooth = 52;  //checks if have signal to connect to phone via bluetooth
const int serialPort = 53;      //checks if have signal to connect to a serial port via bluetooth or USB

int getNegativeSpeed_Port(int num);
int getPositiveSpeed_Port(int num, int recievedVal);
int getSpeed_Port(int num);

int getNegativeSpeed_BT(int num);
int getPositiveSpeed_BT(int num, int recievedVal);
int getSpeed_BT(int num1);

void printFromPort(int printErrors = 0, int printRPMs = 1, int printControlSignals = 0);
void printFromBT(int printErrors = 0, int printRPMs = 1, int printControlSignals = 0);

void saturateControlInput();
void applyControlInput();


void setup() {
  
  Serial.begin(57600);
  Serial1.begin(57600);
  pinMode( 18, INPUT_PULLUP );
  pinMode( 19, INPUT_PULLUP );
  //Set up Motor Driver EN (PWM) and INPUT pins for motor A and B
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  //Communication Source
  pinMode(phoneBluetooth, INPUT);
  pinMode(serialPort, INPUT);
  // Set initial rotation direction
  setMotorAForward();
  setMotorBForward();
  //Initialize the Motor Encoders
  EncoderInit();
  speedA = 0; speedB = 0; speed = 0;
  rpmA = 0; rpmB = 0;

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
  
  ///---------------FOR SERIAL PORT CONNECTION (USB/BLUETOOTH) CONNECTION!!!!!!!
  int num = Serial.available();
  if(num > 0  && digitalRead(serialPort)==HIGH)
  { 
    speed = getSpeed_Port(num);
    //r(t)
    speedA = speed; speedB = speed;
    Serial.read();
  }// end=if statement -- SERIALPORT
    
  ///---------------FOR BLUETOOTH CONNECTION!!!!!!!
  int num1 = Serial1.available();
  if(num1 > 0 && digitalRead(phoneBluetooth)==HIGH)
  {
    if(Serial1.read() == START_BYTE){
      int tempSpeed;
      tempSpeed = getSpeed_BT(num1);
      if(Serial1.read() == END_BYTE){
        speed = tempSpeed;
        speedA = speed; speedB = speed;
      }
    }
    Serial1.read();//to get the carriage return byte
  }// end if statement -- PHONEBLUETOOTH

  
  //e(t)
  errorA = speedA - rpmA; 
  errorB = speedB - rpmB;
  
  errorAIntegral = errorAIntegral + delayTime*errorA/1000.0;
  errorBIntegral = errorBIntegral + delayTime*errorB/1000.0;

  errorADerivative = 1000.0*(errorA-oldErrorA)/delayTime;
  errorBDerivative = 1000.0*(errorB-oldErrorB)/delayTime;
  if(speed == 0){
    errorAIntegral = 0; errorADerivative = 0;
    errorBIntegral = 0; errorBDerivative = 0;
  }

  //u(t)
  conA = KP*errorA + KI*errorAIntegral + KD*errorADerivative;
  conB = KP*errorB + KI*errorBIntegral + KD*errorBDerivative;

  oldErrorA = errorA;
  oldErrorB = errorB;
  
  // Saturate control signal if it goes above/below the largest value for analogWrite
  saturateControlInput();
  applyControlInput();

  rpmA = (durationA/(float)Pulses_Per_Rotation)*60*mills/delayTime;
  rpmB = (durationB/(float)Pulses_Per_Rotation)*60*mills/delayTime;
 
  //if(digitalRead(serialPort)==HIGH) ---Commented out because I want to be able to plot speed even when connected to Phone
  //{
    printFromPort(0, 1, 1);
  //}

  if(digitalRead(phoneBluetooth)==HIGH)
  {
    printFromBT(0, 1, 0);
  }
  
  durationA = 0;
  durationB = 0;

  delay(delayTime);
}// end loop


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

int getSpeed_Port(int num){
  int recievedVal = Serial.read();
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
  num1 = num1 - 2; //'-2' for the START_BYTE and END_BYTE bytes
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


void printFromPort(int printErrors, int printRPMs, int printControlSignals){
  if(printErrors == 1){
    Serial.print("Error A: ");
    Serial.print(errorA);
    Serial.print(",");
    Serial.print("Error B: ");
    Serial.print(errorB);
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
    Serial.print(conA);
    Serial.print(",");
    Serial.print("Control B: ");
    Serial.print(conB);
    Serial.print(",");
  }
  
  Serial.print(" Speed: ");
  Serial.print(speed);
  Serial.println();
}

void printFromBT(int printErrors, int printRPMs, int printControlSignals){
  if(printErrors == 1){
    Serial1.print("Error A: ");
    Serial1.print(errorA);
    Serial1.print(",");
    Serial1.print("Error B: ");
    Serial1.print(errorB);
    Serial1.print(",");
  }
  
  if(printRPMs == 1){
    Serial1.print("RpmA: ");
    Serial1.print(rpmA);
    Serial1.print(",");
    Serial1.print(" Rpm B: ");
    Serial1.print(rpmB);
    Serial1.print(",");
  }
  
  if(printControlSignals == 1){
    Serial1.print("Control A: ");
    Serial1.print(conA);
    Serial1.print(",");
    Serial1.print("Control B: ");
    Serial1.print(conB);
    Serial1.print(",");
  }
  
  Serial1.print(" Speed: ");
  Serial1.print(speed);
  Serial1.println();
}


void saturateControlInput(){
   if(conA > 255)
    conA = 255;
  else if(conA < -255)
    conA = -255;
  if(conB > 255)
    conB = 255;
  else if(conB < -255)
    conB = -255;
}

void applyControlInput(){
  if(conA < 0)
    setMotorAReverse();
  else if(conA > 0)
    setMotorAForward();

  if(conB < 0)
    setMotorBReverse();
  else if(conB > 0)
    setMotorBForward();

  analogWrite(enA, fabs(conA));
  analogWrite(enB, fabs(conB));
}


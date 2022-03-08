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
const int START_BYTE_SPEED = 'a';
const int END_BYTE_SPEED = 'b';

const int START_BYTE_ROT = 'f';
const int END_BYTE_ROT = 'g';

const float R = 3.25;
const float L = 13;

const int phoneBluetooth = 52;  //checks if have signal to connect to phone via bluetooth
const int serialPort = 53;      //checks if have signal to connect to a serial port via bluetooth or USB

int getNegativeSpeed_Port(int num);
int getPositiveSpeed_Port(int num, int recievedVal);
int getSpeed_Port(int num, int recievedVal);

int getNegativeRot_Port(int num);
int getPositiveRot_Port(int num, int recievedVal);
int getRot_Port(int num, int recievedVal);

int getNegativeSpeed_BT(int num);
int getPositiveSpeed_BT(int num, int recievedVal);
int getSpeed_BT(int num1);

int getNegativeRot_BT(int num);
int getPositiveRot_BT(int num, int recievedVal);
int getRot_BT(int num);

void printFromPort(int printErrors = 0, int printRPMs = 1, int printControlSignals = 0);
void printFromBT(int printErrors = 0, int printRPMs = 1, int printControlSignals = 0);

void saturateControlInput();
void applyControlInput();


void setup() {
  
  Serial.begin(115200);
  Serial1.begin(115200);
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
  //Serial.print("First byte speed-");
  //Serial.println(char(recievedValBT));

  if(num1 > 0 && digitalRead(phoneBluetooth)==HIGH )
  {
    if(recievedValBT == START_BYTE_SPEED)
    {
      //Serial.println("Get linear");
      int tempSpeed;
      tempSpeed = getSpeed_BT(num1);
      char dig = Serial1.read();
      if(dig == END_BYTE_SPEED && tempSpeed <= 130 && tempSpeed >= -130){
        speed = tempSpeed;
        //Serial.println("Set linear");
        //speedA = speed; speedB = speed;
      }
      else{
        //Serial.println(tempSpeed);
        //Serial.println(dig);
      }
      Serial1.read();//to get the carriage return byte
    }// end if statement -- PHONEBLUETOOTH
    
    //num1 = Serial1.available();
    //recievedValBT = Serial1.read();
    //Serial.print("First byte rot-");
    //Serial.println(char(recievedValBT));
    if(recievedValBT == START_BYTE_ROT)
    {
      //Serial.println("Get rot");
      int tempRot;
      tempRot = getRot_BT(num1);
      char dig = Serial1.read();
      if(dig == END_BYTE_ROT && tempRot <= 360 && tempRot >= -360){
        rotSpeed = tempRot;
        //Serial.println("Set rot");
        //Serial.print("Got end byte");
      }
      else{
        //Serial.println(tempRot);
        //Serial.println(dig);
      }
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

  
  //e(t)
  errorA = speedA - rpmA; 
  errorB = speedB - rpmB;
  
  errorAIntegral = errorAIntegral + delayTime*errorA/1000.0;
  errorBIntegral = errorBIntegral + delayTime*errorB/1000.0;

  errorADerivative = 1000.0*(errorA-oldErrorA)/delayTime;
  errorBDerivative = 1000.0*(errorB-oldErrorB)/delayTime;
  if(speed == 0 && rotSpeed == 0){
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

  actualRotSpeed = (rpmB-rpmA)*1.5;
 
  //if(digitalRead(serialPort)==HIGH) ---Commented out because I want to be able to plot speed even when connected to Phone
  //{
    printFromPort(0, 1, 0);
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
    Serial1.print(errorA);
    Serial1.print(",");
    Serial1.print("Error B: ");
    Serial1.print(errorB);
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
    Serial1.print(conA);
    Serial1.print(",");
    Serial1.print("Control B: ");
    Serial1.print(conB);
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


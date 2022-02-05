#include <Arduino.h>

//PWM pins for MotorA and MotorB respectively.  They control the motor speed.
const int enA = 9; //Enable A
const int enB = 10;//Enable B

//Input Pins for MotorA and B.  They control the direction of the motors
const int in1 = 7;//Inputs for Motor A
const int in2 = 6;
const int in3 = 5;//Inputs for Motor B
const int in4 = 4;

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

const int Pulses_Per_Rotation = 1920; //In the wiki it states there are 960 pulses, but we are checking change in states
// i.e. rising edge and falliing, edge therefore 1920 = 960*2

const int mills = 1000;
const int delayTime = 250; 

int speedA = 0;
int speedB = 0;
int speed;

void EncoderInit();
void wheelSpeedA();
void wheelSpeedB();
void setMotorAForward();
void setMotorBForward();
void setMotorAReverse();
void setMotorBReverse();


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  //Serial.print("MotorA speed is: ");
  //Serial.println(speedA);
  //Serial.print("MotorB speed is: ");
  //Serial.println(speedB);
  
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);

  pinMode(enB, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  // Set initial rotation direction
  setMotorAForward();
  setMotorBForward();

  EncoderInit();//Initialize the module

}

void loop() {
  // put your main code here, to run repeatedly:

  int num = Serial.available();
  if(num > 0){ 
    //Serial.print("Number of bytes sent is ");
    //Serial.println(num);

    int recievedVal = Serial.read();
    //Serial.print("The recieved Value is ");
    //Serial.println(recievedVal);

    if(recievedVal == '0'){// Set motor speed to 0
      speedA = 0;
      speedB = 0;
      analogWrite(enA, speedA);
      analogWrite(enB, speedB);
    }
    else if (recievedVal == 'r'){//Place motors in reverse motion
      setMotorAReverse();
      setMotorBReverse();
      //Serial.println("Reverse Motors");
    }
    else if (recievedVal == 'f'){//place motors in forward motion
      setMotorAForward();
      setMotorBForward();
      //Serial.println("Forward Motors");
    }
    else if (recievedVal >= '1' && recievedVal <= '9'){//A non-zero motor speed was entered
      byte digits = num - 1;
      if(digits == 1){//only one digit was entered
        speed = recievedVal - '0';
      }
      else if(digits == 2){//two digits were entered
        int value = Serial.read();
        speed = 10*(recievedVal - '0') + value - '0';
      }
      else if(digits == 3){//three digits were entered
        int value2 = Serial.read(), value3 = Serial.read();
        speed = 100*(recievedVal - '0') + 10*(value2 -'0') +  value3- '0';
      }

      speedA = speed; speedB = speed;
      analogWrite(enA, speedA);
      analogWrite(enB, speedB);
      //Serial.print("MotorA speed is: ");
      //Serial.println(speedA);
      //Serial.print("MotorB speed is: ");
      //Serial.println(speedB);
    }
  Serial.read();//to get the carriage return byte
  }

  
  
  rpmA = (durationA/(float)Pulses_Per_Rotation)*60*mills/delayTime;
  rpmB = (durationB/(float)Pulses_Per_Rotation)*60*mills/delayTime;

  Serial.print("Pulses A: ");
  Serial.print(durationA);
  Serial.print(",");
  Serial.print("Rpm A: ");
  Serial.print(rpmA);
  Serial.print(",");
  Serial.print("Direction A: ");
  Serial.print(DirectionA);
  Serial.print(",");
  Serial.print("Pulses B: ");
  Serial.print(durationB);
  Serial.print(",");
  Serial.print("Rpm B: ");
  Serial.print(rpmB);
  Serial.print(",");
  Serial.print("Direction B: ");
  Serial.print(DirectionB);
  Serial.println();
  
  
  durationA = 0;
  durationB = 0;
  delay(delayTime);
}

void EncoderInit()
{
  DirectionA = true;//default -> Forward
  DirectionB = true;
  pinMode(encA2,INPUT);
  pinMode(encB2, INPUT);
  attachInterrupt(digitalPinToInterrupt(encA1), wheelSpeedA, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encB1), wheelSpeedB, CHANGE);
}

/**
 * ISR.  Incrememnts/decrememnts the number of pulses for when the motor is rotating forward/backward
 * On rising edge it verifies the direction of the motor.
 */
void wheelSpeedA()
{
  int current_A1 = digitalRead(encA1);
  if((previous_A1 == LOW) && current_A1==HIGH)//only checking direction when A1 rises from LOW to HIGH
  {
    int val_A2 = digitalRead(encA2);
    if(val_A2 == LOW && DirectionA)//Checks if wheel is no longer moving forward and is now in reverse
    {
      DirectionA = false; //Reverse
    }
    else if(val_A2 == HIGH && !DirectionA)//Checks if wheel is no longer reversing and is now moving forward
    {
      DirectionA = true;  //Forward
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
      DirectionB = true;
    }
    else if(val_B2 == HIGH && !DirectionB)//Checks if wheel is no longer reversing and is now moving forward
    {
      DirectionB = false;
    }
  }
  previous_B1 = current_B1;
  

  if(DirectionB)  durationB++;
  else  durationB--;
}

void setMotorAReverse(){
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  DirectionA = false;
}

void setMotorBReverse(){
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  DirectionB = false;
}

void setMotorAForward(){
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  DirectionA = true;
}

void setMotorBForward(){
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  DirectionB = true;
}
#include <Arduino.h>

//PWM pins for MotorA and MotorB respectively.  They control the motor speed.
const byte enA = 9; //Enable A
const byte enB = 10;//Enable B

//Input Pins for MotorA and B.  They control the direction of the motors
const byte in1 = 7;//Inputs for Motor A
const byte in2 = 6;
const byte in3 = 5;//Inputs for Motor B
const byte in4 = 4;


const byte encA_A = 2;//A Pin -> the interrupt pin 0
const byte encA_B = 8;//B Pin
byte encoder0PinALast;
int duration = 0;//the number of the pulses
float rpm = 0;
boolean Direction;//the rotation direction


const int Pulses_Per_Rotation = 1920;

const int mills = 1000;
const int delayTime = 250; 

const byte Cool_Term_Min_Input_Byte_Size = 3;

int rotDirection = 0;
int speedA = 0;
int speedB = 0;
int speed;

void EncoderInit();
void wheelSpeed();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.print("MotorA speed is: ");
  Serial.println(speedA);
  Serial.print("MotorB speed is: ");
  Serial.println(speedB);
  
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);

  pinMode(enB, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  // Set initial rotation direction
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  Serial.println("Enter Motor Speed: ");

  EncoderInit();//Initialize the module

}

void loop() {
  // put your main code here, to run repeatedly:

  analogWrite(enA, speedA);
  analogWrite(enB, speedB);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  //Serial.println(speedA);
  //Serial.println(speedB);

  int num = Serial.available();
  if(num > Cool_Term_Min_Input_Byte_Size){ //set to 2 to avoid false positives when using CoolTerm to communicate over the Serial Port
    Serial.print("Number of bytes sent is ");
    Serial.println(num);
    speed = Serial.parseInt();
    speedA = speed;
    speedB = speed;
    Serial.print("MotorA speed is: ");
    Serial.println(speedA);
    Serial.print("MotorB speed is: ");
    Serial.println(speedB);
    Serial.println("Enter Motor Speed: ");
  }
  //else{
  //  Serial.println("There is no Serial Input");
  //}
  Serial.print("Pulse: ");
  Serial.print(duration);

  rpm = (duration/(float)Pulses_Per_Rotation)*60*mills/delayTime;
  Serial.print("     RPM: ");
  Serial.println(rpm);
  duration = 0;
  delay(delayTime);
}

void EncoderInit()
{
  Direction = true;//default -> Forward
  pinMode(encA_B,INPUT);
  attachInterrupt(0, wheelSpeed, CHANGE);
}

void wheelSpeed()
{
  int Lstate = digitalRead(encA_A);
  if((encoder0PinALast == LOW) && Lstate==HIGH)
  {
    int val = digitalRead(encA_B);
    if(val == LOW && Direction)
    {
      Direction = false; //Reverse
    }
    else if(val == HIGH && !Direction)
    {
      Direction = true;  //Forward
    }
  }
  encoder0PinALast = Lstate;

  if(!Direction)  duration++;
  else  duration--;
}
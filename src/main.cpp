#include <Arduino.h>

const byte enA = 9;
const byte enB = 10;
const byte in1 = 7;
const byte in2 = 6;

const byte in3 = 5;
const byte in4 = 4;

const byte Cool_Term_Min_Input_Byte_Size = 3;

int rotDirection = 0;
int speedA = 120;
int speedB = 220;
int speed;

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

}

void loop() {
  // put your main code here, to run repeatedly:

  analogWrite(enA, speedA);
  analogWrite(enB, speedB);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  Serial.println(speedA);
  Serial.println(speedB);

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
  

  delay(500);
  

}

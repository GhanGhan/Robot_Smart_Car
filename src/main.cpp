#include <Arduino.h>

#define enA 9
#define enB 10
#define in1 7
#define in2 6

#define in3 5
#define in4 4

int rotDirection = 0;
int speedA = 120;
int speedB = 220;

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

}

void loop() {
  // put your main code here, to run repeatedly:

  analogWrite(enA, speedA);
  analogWrite(enB, speedB);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  delay(500);
  

}

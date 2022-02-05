#include <Arduino.h>

//PWM pins for MotorA and MotorB respectively.  They control the motor speed.
const byte enA = 9; //Enable A
const byte enB = 10;//Enable B

//Input Pins for MotorA and B.  They control the direction of the motors
const byte in1 = 7;//Inputs for Motor A
const byte in2 = 6;
const byte in3 = 5;//Inputs for Motor B
const byte in4 = 4;

//Encoder pins for Motor A
const byte encA1 = 2;//A Pin -> the interrupt pin 0
const byte encA2 = 8;//B Pin
byte previous_A1;
volatile int duration = 0;//the number of the pulses
float rpm = 0;
boolean Direction;//the rotation direction


const int Pulses_Per_Rotation = 1920; //In the wiki it states there are 960 pulses, but we are checking change in states
// i.e. rising edge and falliing, edge therefore 1920 = 960*2

const int mills = 1000;
const int delayTime = 250; 

int rotDirection = 0;
int speedA = 0;
int speedB = 0;
int speed;

void EncoderInit();
void wheelSpeed();
void setMotorAForward();
void setMotorBForward();
void setMotorAReverse();
void setMotorBReverse();


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
  setMotorAForward();
  setMotorBForward();

  EncoderInit();//Initialize the module

}

void loop() {
  // put your main code here, to run repeatedly:

  int num = Serial.available();
  if(num > 0){ 
    Serial.print("Number of bytes sent is ");
    Serial.println(num);

    int recievedVal = Serial.read();
    Serial.print("The recieved Value is ");
    Serial.println(recievedVal);

    if(recievedVal == '0'){// Set motor speed to 0
      speedA = 0;
      speedB = 0;
      analogWrite(enA, speedA);
      analogWrite(enB, speedB);
    }
    else if (recievedVal == 'r'){//Place motors in reverse motion
      setMotorAReverse();
      setMotorBReverse();
      Serial.print("Reverse Motors");
    }
    else if (recievedVal == 'f'){//place motors in forward motion
      setMotorAForward();
      setMotorBForward();
      Serial.print("forward Motors");
    }
    else if (recievedVal >= '1' && recievedVal <= '9'){//A non-zero motor speed was entered
      byte digits = num - 1;
      if(digits == 1){//only one digit was entered
        speed = recievedVal - '0';
      }
      else if(digits == 2){//two digits were entered
        int value = Serial.read();
        speed = 10*(recievedVal - '0') + value- '0';
      }
      else if(digits == 3){//three digits were entered
        int value2 = Serial.read(), value3 = Serial.read();
        speed = 100*(recievedVal - '0') + 10*(value2 -'0') +  value3- '0';
      }

      Serial.read();//to get the carriage return bit
      
      speedA = speed; speedB = speed;
      analogWrite(enA, speedA);
      analogWrite(enB, speedB);
      Serial.print("MotorA speed is: ");
      Serial.println(speedA);
      Serial.print("MotorB speed is: ");
      Serial.println(speedB);
    }
    


  }
  
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
  pinMode(encA2,INPUT);
  attachInterrupt(0, wheelSpeed, CHANGE);
}

/**
 * ISR.  Incrememnts/decrememnts the number of pulses for when the motor is rotating forward/backward
 * On rising edge it verifies the direction of the motor.
 */
void wheelSpeed()
{
  int current_A1 = digitalRead(encA1);
  if((previous_A1 == LOW) && current_A1==HIGH)//only checking direction when A1 rises from LOW to HIGH
  {
    int val_A2 = digitalRead(encA2);
    if(val_A2 == LOW && Direction)//Checks if wheel is no longer moving forward and is now in reverse
    {
      Direction = false; //Reverse
    }
    else if(val_A2 == HIGH && !Direction)//Checks if wheel is no longer reversing and is now moving forward
    {
      Direction = true;  //Forward
    }
  }
  previous_A1 = current_A1;

  if(!Direction)  duration++;
  else  duration--;
}

void setMotorAForward(){
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
}

void setMotorBForward(){
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void setMotorAReverse(){
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
}

void setMotorBReverse(){
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}
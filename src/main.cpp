#include <Arduino.h>
#include "wheelEncoder.h"

float Kp = 10;
float Ki = 40;
float errorA, errorB, conA, conB;
float errorAI = 0, errorBI = 0;

void setup() {
  
  Serial.begin(57600);
  //Set up Motor Driver EN (PWM) and INPUT pins for motor A and B
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  // Set initial rotation direction
  setMotorAForward();
  setMotorBForward();
  //Initialize the Motor Encoders
  EncoderInit();
  speedA = 0; speedB = 0; speed = 0;
  rpmA = 0; rpmB = 0;

}

void loop() {
  
  //used to test noise of speed acquisition of the encoders, very little if any noise, LPF may not be required
  //we will go straight to PID
  /*
  if(speed < 255){
    speed += 5;
    speedA = speed;
    speedB = speed;
    analogWrite(enA, speedA);
    analogWrite(enB, speedB);
  }
  */

  int num = Serial.available();
  if(num > 0){ 
    //Serial.print("Number of bytes sent is ");
    //Serial.println(num);

    int recievedVal = Serial.read();

    if(recievedVal == '0'){// Set motor speed to 0
      speedA = 0;
      speedB = 0;
      speed = 0;
      analogWrite(enA, speedA);
      analogWrite(enB, speedB);
    }
    else if (recievedVal == 'r'){//Place motors in reverse motion
      setMotorAReverse();
      setMotorBReverse();
    }
    else if (recievedVal == 'f'){//place motors in forward motion
      setMotorAForward();
      setMotorBForward();
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
      //r(t)
      speedA = speed; speedB = speed;

      
      
    }//end recievedVal if-else-if statements

  Serial.read();//to get the carriage return byte

  }// end num if statement

  
  //e(t)
  errorA = speedA - rpmA; 
  errorB = speedB - rpmB;
  
  errorAI = errorAI + delayTime*errorA/1000.0;
  errorBI = errorBI + delayTime*errorB/1000.0;

  //u(t)
  conA = Kp*errorA + Ki*errorAI;
  conB = Kp*errorB + Ki*errorBI;

  if(conA < 0)
  {
    setMotorAReverse();
  }
  else
  {
    setMotorAForward();
  }

  if(conB < 0)
  {
    setMotorBReverse();
  }
  else
  {
    setMotorBForward();
  }

  if(fabs(conA) > 255)
    conA = 255;
  if(fabs(conB) > 255)
    conB = 255;
  
  analogWrite(enA, fabs(conA));
  analogWrite(enB, fabs(conB));
  
  rpmA = (durationA/(float)Pulses_Per_Rotation)*60*mills/delayTime;
  rpmB = (durationB/(float)Pulses_Per_Rotation)*60*mills/delayTime;

  Serial.print("Error A: ");
  Serial.print(errorA);
  Serial.print(",");
  Serial.print("Rpm A: ");
  Serial.print(rpmA);
  //Serial.print(",");
  //Serial.print("Control A: ");
  //Serial.print(conA);
  Serial.print(",");
  Serial.print("Error B: ");
  Serial.print(errorB);
  Serial.print(",");
  Serial.print("Rpm B: ");
  Serial.print(rpmB);
  Serial.print(",");
  Serial.print("Speed: ");
  Serial.print(speed);
  //Serial.print(",");
  //Serial.print("Control B: ");
  //Serial.print(conB);
  Serial.println();
  
  durationA = 0;
  durationB = 0;
  delay(delayTime);
}// end loop


#include <Arduino.h>
#include "wheelEncoder.h"

void setup() {
  
  Serial.begin(9600);
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

}

void loop() {
  
  int num = Serial.available();
  if(num > 0){ 
    //Serial.print("Number of bytes sent is ");
    //Serial.println(num);

    int recievedVal = Serial.read();

    if(recievedVal == '0'){// Set motor speed to 0
      speedA = 0;
      speedB = 0;
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

      speedA = speed; speedB = speed;
      analogWrite(enA, speedA);
      analogWrite(enB, speedB);
    }//end recievedVal if-else-if statements

  Serial.read();//to get the carriage return byte

  }// end num if statement

  
  
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
}// end loop


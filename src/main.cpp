#include <Arduino.h>
#include "wheelEncoder.h"

float Kp = 6.0;//8.5;
float Ki = 6;//5;
float Kd = 0.02;
float errorA, errorB, conA, conB;
float errorAIntegral = 0, errorBIntegral = 0;
float errorADerivative = 0, errorBDerivative = 0;
float step = 0.2;
int append = 'a';
int prepend = 'b';

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
  speed = 130.0*sin(step);
  speedA = speed; speedB = speed;
  step = step+0.2;
  */
  
  int num = Serial.available();
  if(num > 0){ 
    //Serial.print("Number of bytes sent is ");
    //Serial.println(num);
    ///---------------FOR USB CONNECTION!!!!!!!
    
    int recievedVal = Serial.read();
    /*
    if(recievedVal == '0'){// Set motor speed to 0
      speedA = 0;
      speedB = 0;
      speed = 0;
    }
    else if (recievedVal == 'r'){//Place motors in reverse motion
      setMotorAReverse();
      setMotorBReverse();
    }
    else if (recievedVal == 'f'){//place motors in forward motion
      setMotorAForward();
      setMotorBForward();
    }
    else if(recievedVal == '-'){//A non-zero negative motor speed
      byte digits = num - 2;
      if(digits == 1){//only one digit was entered
        int value1 = Serial.read();
        speed = -(value1 - '0');
      }
      else if(digits == 2){//two digits were entered
        int value1 = Serial.read(), value2 = Serial.read();
        speed = -(10*(value1 - '0') + value2 - '0');
      }
      else if(digits == 3){//three digits were entered
        int value1 = Serial.read(), value2 = Serial.read(), value3 = Serial.read();
        speed = -(100*(value1 - '0') + 10*(value2 -'0') +  value3 - '0');
      }
      //r(t)
      speedA = speed; speedB = speed;
    }
    else if (recievedVal >= '1' && recievedVal <= '9'){//A non-zero motor positive speed was entered
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
    */
    ///---------------FOR BLUETOOTH CONNECTION!!!!!!!
    ///*
    int tempSpeed;
    if(recievedVal == append)
    {
      recievedVal = Serial.read();
      if(recievedVal == '0'){// Set motor speed to 0
        tempSpeed = 0;
      }
      else if (recievedVal == 'r'){//Place motors in reverse motion
        setMotorAReverse();
        setMotorBReverse();
      }
      else if (recievedVal == 'f'){//place motors in forward motion
        setMotorAForward();
        setMotorBForward();
      }
      else if(recievedVal == '-'){//A non-zero negative motor speed
        byte digits = num - 2 -2; //last '-2' for the append and prepend bytes
        if(digits == 1){//only one digit was entered
          int value1 = Serial.read();
          tempSpeed = -(value1 - '0');
        }
        else if(digits == 2){//two digits were entered
          int value1 = Serial.read(), value2 = Serial.read();
          tempSpeed = -(10*(value1 - '0') + value2 - '0');
        }
        else if(digits == 3){//three digits were entered
          int value1 = Serial.read(), value2 = Serial.read(), value3 = Serial.read();
         tempSpeed = -(100*(value1 - '0') + 10*(value2 -'0') +  value3 - '0');
        }
      }
      else if (recievedVal >= '1' && recievedVal <= '9'){//A non-zero motor positive speed was entered
        byte digits = num - 1 -2; //last '-2' for the append and prepend bytes;
        if(digits == 1){//only one digit was entered
          tempSpeed = recievedVal - '0';
        }
        else if(digits == 2){//two digits were entered
          int value = Serial.read();
          tempSpeed = 10*(recievedVal - '0') + value - '0';
        }
        else if(digits == 3){//three digits were entered
          int value2 = Serial.read(), value3 = Serial.read();
          tempSpeed = 100*(recievedVal - '0') + 10*(value2 -'0') +  value3- '0';
        }
      }
      if(Serial.read() == 'b'){
        speed = tempSpeed;
        speedA = speed;
        speedB = speed;
      }
    }
    //*/
    Serial.read();//to get the carriage return byte
    

  }// end num if statement

  
  //e(t)
  errorA = speedA - rpmA; 
  errorB = speedB - rpmB;
  
  errorAIntegral = errorAIntegral + delayTime*errorA/1000.0;
  errorBIntegral = errorBIntegral + delayTime*errorB/1000.0;

  errorADerivative = 1000.0*errorA/delayTime;
  errorBDerivative = 1000.0*errorB/delayTime;
  if(speed == 0){
    errorAIntegral = 0; errorADerivative = 0;
    errorBIntegral = 0; errorBDerivative = 0;
  }


  //u(t)
  conA = Kp*errorA + Ki*errorAIntegral + Kd*errorADerivative;
  conB = Kp*errorB + Ki*errorBIntegral + Kd*errorBDerivative;

  
  if(conA > 255)
    conA = 255;
  else if(conA < -255)
    conA = -255;
  if(conB > 255)
    conB = 255;
  else if(conB < -255)
    conB = -255;
  
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
  
  
  /*
  if(speedA < 0)
    setMotorAReverse();
  else if(speedA > 0)
    setMotorAForward();

  if(speedB < 0)
    setMotorBReverse();
  else if(speedB > 0)
    setMotorBForward();

  analogWrite(enA, abs(speedA));
  analogWrite(enB, abs(speedB));
  */
  
 
  rpmA = (durationA/(float)Pulses_Per_Rotation)*60*mills/delayTime;
  rpmB = (durationB/(float)Pulses_Per_Rotation)*60*mills/delayTime;
 

  //Serial.print("Error A: ");
  //Serial.print(errorA);
  //Serial.print(",");
  Serial.print("RpmA: ");
  Serial.print(rpmA);
  //Serial.print(",");
  //Serial.print("Control A: ");
  //Serial.print(conA);
  //Serial.print(",");
  //Serial.print("Direction A: ");
  //Serial.print(DirectionA);
  //Serial.print(",");
  //Serial.print("Error B: ");
  //Serial.print(errorB);
  Serial.print(",");
  Serial.print(" Rpm B: ");
  Serial.print(rpmB);
  Serial.print(",");
  Serial.print(" Speed: ");
  Serial.print(speed);
  //Serial.print(",");
  //Serial.print("Control B: ");
  //Serial.print(conB);
  //Serial.print(",");
  //Serial.print("Direction B: ");
  //Serial.print(DirectionB);
  Serial.println();
  
  durationA = 0;
  durationB = 0;

  delay(delayTime);
}// end loop


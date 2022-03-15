#include "Arduino.h"
#include "Encoder.h"

Encoder * Encoder::myInstance[MAX_WHEELS] = {nullptr, nullptr};

Encoder::Encoder()
{}

Encoder::~Encoder()
{
    detachInterrupt(digitalPinToInterrupt(encoderPinA_Int));
    myInstance[wheelID] = nullptr;
}


float Encoder::getRPM()
{
    return (pulses/(float)pulses_per_rotation)*60*1000/delayTime;
}

void Encoder::setPulsesToZeros()
{
    pulses = 0;
}


Encoder::Encoder(int encoderA, int encoderB, int totalPulses, int delayTimeMills)
{
    pulses_per_rotation = totalPulses;
    encoderPinA_Int = encoderA;
    encoderPinB = encoderB;
    delayTime = delayTimeMills;
    previous_A = 0;
    pulses = 0;
    mode[0] = CHANGE;
    mode[2] = RISING;
    mode[3] = FALLING;
}

// ISR for each myISRId
void Encoder::globalISR0(void) { Encoder::myInstance[0]->wheelSpeed(); }
void Encoder::globalISR1(void) { Encoder::myInstance[1]->wheelSpeed(); }

void Encoder::EncoderInit(int wheelNumber, int type)
{
    direction = true;
    pinMode(encoderPinB, INPUT);
    wheelID = wheelNumber;
    if(myInstance[wheelID] == nullptr)
    {
        myInstance[wheelID] = this;
        switch(wheelID){
            case 0:
                attachInterrupt(digitalPinToInterrupt(encoderPinA_Int), globalISR0, mode[type]);
                break;
            case 1:
                attachInterrupt(digitalPinToInterrupt(encoderPinA_Int), globalISR1, mode[type]);
                break;
        }
    }
    else
    {
        Serial.print("WHEEL_ID: ");
        Serial.print(wheelID);
        Serial.println("HAS ALREADY BEEN ASSIGNED!!!!!!");
    }
}

void Encoder::wheelSpeed()
{
    int currentA = digitalRead(encoderPinA_Int);
    if(wheelID == 0)
    {
        if((previous_A == LOW) && currentA==HIGH)//only checking direction when A1 rises from LOW to HIGH
        {
            int val_B = digitalRead(encoderPinB);
            if(val_B == LOW && !direction)//Checks if wheel is no longer moving forward and is now in reverse
            {
                direction = true;//false; //Reverse
            }
            else if (val_B == HIGH && direction)//Checks if wheel is no longer reversing and is now moving forward
            {
                direction = false;//true;  //Forward
            }
        }       
        previous_A = currentA;

        if(direction)  pulses++;
            else  pulses--;
    }
    else if(wheelID == 1)
    {
        if((previous_A == LOW) && currentA==HIGH)//only checking direction when B1 rises from LOW to HIGH
        {
            int val_B = digitalRead(encoderPinB);
            if(val_B == LOW && direction)//Checks if wheel is no longer moving forward and is now in reverse
            {
                direction = false;//true;
            }
            else if(val_B == HIGH && !direction)//Checks if wheel is no longer reversing and is now moving forward
            {
                direction = true;//false;
            }
        }
        previous_A = currentA;
  
        if(direction)  pulses++;
        else  pulses--;
    }
}
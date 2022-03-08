#ifndef _wheelEncoder_h
#define _wheelEcoder_h

//PWM pins for MotorA and MotorB respectively.  They control the motor speed.
extern const int enA; //Enable A
extern const int enB;//Enable B

//Input Pins for MotorA and B.  They control the direction of the motors
extern const int in1;//Inputs for Motor A
extern const int in2;
extern const int in3;//Inputs for Motor B
extern const int in4;

//Encoder pins for Motor A
extern const int encA1;//A Pin -> the interrupt pin 0
extern const int encA2;//B Pin
extern int previous_A1;
extern volatile int durationA;//the number of the pulses
extern float rpmA;
extern volatile bool DirectionA;//the rotation direction

//Encoder pins for Motor B
extern const int encB1;//A Pin -> the interrupt pin 1
extern const int encB2;//B Pin
extern int previous_B1;
extern volatile int durationB;//the number of the pulses
extern float rpmB;
extern volatile bool DirectionB;//the rotation direction

extern const int Pulses_Per_Rotation; //In the wiki it states there are 960 pulses, but we are checking change in states
// i.e. rising edge and falliing, edge therefore 1920 = 960*2

extern const int mills;
extern const int delayTime; 

extern float speedA;
extern float speedB;
extern float speed;


extern float rotSpeed;
extern float actualRotSpeed;

/**
 * Attaches the ISR for calculating the wheelspade of Motor A and Motor B to the appropriate encoder pins.
 * Also sets the secondary encoder pins for each pair as INPUT pins
 */
void EncoderInit();

/**
 * ISR.  Incrememnts/decrememnts the number of pulses for when the motor is rotating forward/backward
 * On rising edge it verifies the direction of the motor.  This is for Motor A.
 */
void wheelSpeedA();

/**
 * ISR.  Incrememnts/decrememnts the number of pulses for when the motor is rotating forward/backward
 * On rising edge it verifies the direction of the motor.  This is for Motor B.
 */
void wheelSpeedB();

/**
 * Places the motor driver inputs into a state that cause the Wheel A to rotate forward.
 * 
 */
void setMotorAForward();

/**
 * Places the motor driver inputs into a state that cause the Wheel B to rotate forward.
 * 
 */
void setMotorBForward();

/**
 * Places the motor driver inputs into a state that cause the Wheel A to rotate in reverse.
 * 
 */
void setMotorAReverse();

/**
 * Places the motor driver inputs into a state that cause the Wheel B to rotate in reverse.
 * 
 */
void setMotorBReverse();


#endif
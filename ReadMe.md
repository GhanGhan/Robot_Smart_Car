# Overview
The robot car is a two-wheel differential-drive mobile robot.  The wheels are actuated by DC motors whose speed and direction are sensed using quadrature encoders.  The linear and angular velocity commands for the car can be sent over USB or Bluetooth.  An associated phone app was created that allows for remote control of the car via Bluetooth.  In order to quickly get to and maintain the desired linear and angular velocities a PID control system was implemented.  An Arduino Mega is used as the microcontroller for the car.

# Table of Contents
1. Summary of Functionality
2. Repository Structure
3. Code Description
4. Installation
5. License
6. Acknowledgments

# Summary of Functionality
-	Hardware switch to change velocity command source between USB and Bluetooth with indicator lights to signal which medium to robot is expecting a command to originate from
-	Direction and speed of each wheel controlled by separate PID (Proportional-Integral-Differential) control system
-	Maximum linear of the robot speed is set to 130 rpm, given the radius of the wheel that is 44.24 cm/s
-	Maximum rotational speed of the robot is set to 200 deg/s
-	Phone app uses slide switches to control robots linear and angular velocity, can immediatly stop linear motion, rotational motion or both using buttons
-	App adds start and stop bits to velocity commands so that corrupted values are ignored are ignored
-	App has window where user can see the current velocity of the robot and the velocity commands currently recieved by the robot

# Repository Structure
```
├── Documents
│   ├── 7_5V Step Responses.xlsx
│   ├── Bill of Materials.xlsx
│   ├── Motor Speed Vs PWM and Voltage.xlsx
│   ├── Remote Control Robot Car.docx
│   ├── Response Test.xlsx
│   └── Time Metrics.xlsx
├── include
│   ├── Encoder.h
│   ├── MotorDriver.h
│   ├── PID.h
│   ├── README
│   └── SerialInputParsing.h
├── lib
│   └── README
├── include
│   ├── Encoder.cpp
│   ├── MotorDriver.cpp
│   ├── PID.cpp
│   ├── SerialInputParsing.cpp
│   └── main.cpp
├── test
│   └── README
└── platformio.ini
```
# Code Description
- **PID:**
   - Calculates the control signal that should be sent to the motor drivers based on the error between the desired and current velocity of the motors
- **Encoder:**
   - Calculates the current speed of the motor and determines their rotational direction using inputs from the quadrature encoders
- **MotorDriver:**
   - Enables/Disables the motors and sends the control signals that sets the motors speed & direction of rotation
- **SerialInputParsing:**
   - Determines the value of the velocity sent from the command source, Bluetooth or USB
   - Contains error detection code to sense of the recieved data is corrupted, in which case it will be thrown out
- The .h files contain the class, functions and data structure declarations
- The .c files manipulate the member variables, usually by applying algorithms and/or logic of varying complexity
- **The test folder is intended for PlatformIO Unit Testing and project tests & the lib folder is intended to be used for project specific (private) libraries, neither is being used as of now
- **main.c:**
   - Instantiates timing, PID, motor control, encoder, speed, error detection and command source detection parameters
   - Setup communication channels, enable motors and motor directions
   - Done in a loop:
       - Check if connected to Bluetooth or USB
       - Get velocity command, ensure it is uncorropted, if it is throw it out
       - Acquire current wheel velocity from the encoders, use this the the velocity command to calculate the next control signal
       - Apply control signal to the motor drivers
       - Send current linear and rotational velocity values to command source for printing
- **platformio.ini:**
   - The configuration file that lets you set up your development environment, share your code and configurations with others, and leverage a declarative approach to development
   - Can use platformio.ini to configure multiple platforms and architectures without having to worry about different toolchains or build systems for each one.

# Installation
- This project was primarily made using Visual Studio code and PlatformIO, to download both follow this guide from the [DroneBotWorkshop](https://dronebotworkshop.com/platformio/)
- Can also use the (Arduino IDE 2)[https://docs.arduino.cc/software/ide-v2], follow this guide for dowloading and getting started
    - This was useful for Serial plotting and debugging
 
# License
- [![License: CC BY 3.0](https://img.shields.io/badge/License-CC%20BY%203.0-lightgrey.svg)](https://creativecommons.org/licenses/by/3.0/)
- This project is licensed under the [Creative Commons Attribution 3.0 Unported License](https://creativecommons.org/licenses/by/3.0/).
  
# Acknoledgments
- I would like to thank the DroneBotWorkshop for notifying me about PlatformIO and helping me set it up

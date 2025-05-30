# Overview
The robot car is a two-wheel differential-drive mobile robot.    An Arduino Mega is used as the microcontroller for the car as shown in *Figure 1*.  The wheels are actuated by DC motors whose speed and direction are sensed using quadrature encoders as shown in *Figure 2*.  The linear and angular velocity commands for the car can be sent over USB or Bluetooth.  I connected the robot to my laptop over USB to program the Arduino and to test the control system behaviour while the robot was stationary.  An associated phone app was created that allows for remote control of the car via Bluetooth.  In order to quickly get to and maintain the desired linear and angular velocities a PID control system was implemented.

![RoboComponents1](https://github.com/GhanGhan/Robot_Smart_Car/assets/17633599/79900a40-3f25-49c4-aa56-31075ebf370c)
***Figure 1: Robot Components Top View***

![RoboComponents2](https://github.com/GhanGhan/Robot_Smart_Car/assets/17633599/e374f934-4ac6-477c-83cc-e5df01be7d38)
***Figure 2: Robot Components Auxilliary View***

# Table of Contents
1. Summary of Functionality
2. Prototyping and Operation Videos
3. Repository Structure
4. Code Description
5. Installation
6. License
7. Acknowledgments

# Summary of Functionality
-	Hardware switch to change velocity command source between USB and Bluetooth with indicator lights to signal which medium to robot is expecting a command to originate from (see *Figure 1*)
-	Phone app uses slide switches to control robots linear and angular velocity, can immediatly stop linear motion, rotational motion or both using buttons as shown in *Figure 3*
-	App adds start and stop bits to velocity commands so that corrupted values are ignored are ignored
-	App has window where user can see the current velocity of the robot and the velocity commands currently recieved by the robot as shown in *Figure 3*
-  The velocity of each wheel is controlled by a PID (Proportional-Integral-Differential) control system, one for each wheel, the resulting linear and angular velocity of the robot given the individual wheel velocities are shown in *Figure 4*
   - **I programmed the PID controller from the ground up, no third party library was used!!!**
   - Gains: Kp = 5.75, Ki = 7, Kd = 0.02
-	Maximum linear of the robot speed is set to 130 rpm, given the radius of the wheel that is 0.4424 m/s, *Figure 4* details the robot velocity calculations
-	Maximum rotational speed of the robot is set to 200 deg/s
![Controller_Enlarged](https://github.com/GhanGhan/Robot_Smart_Car/assets/17633599/b98a65c2-907a-44a8-942d-68309febd8c5)
***Figure 3: Phone App User Interface***

![DifferentialDrive](https://github.com/GhanGhan/Robot_Smart_Car/assets/17633599/81d85615-7332-4068-91cf-981762f15719)
***Figure 4: Linear and Angular Velocity Equations***

# Prototyping and Operation Videos

[![VidPic1_25%](https://github.com/GhanGhan/Robot_Smart_Car/assets/17633599/1c135c02-4902-49fa-944f-2a51f0117150)](https://www.youtube.com/watch?v=zZjfyD7H7h0)
[![VidPic2_25%](https://github.com/GhanGhan/Robot_Smart_Car/assets/17633599/860daa63-4580-47f3-8783-7e725af58f42)](https://www.youtube.com/watch?v=FXt78br-hj0)

# Response Plots
## Sinusoidal Inputs
![50sin](https://github.com/user-attachments/assets/3fbddac4-f256-4f11-85d9-4453e9f535ff)
![130sin](https://github.com/user-attachments/assets/e964c6a3-875c-435e-9a6c-78afce7ba43c)

## Ramp Input
![Ramp](https://github.com/user-attachments/assets/9d647bd0-0dfb-46f3-9678-99afd95142cc)

## Step Inputs
![StepUpTo130](https://github.com/user-attachments/assets/1d8a5c0d-d103-465d-bcd2-d4f097dda70e)
![StepDownTo0](https://github.com/user-attachments/assets/cb410152-55f1-4b3b-ad35-5dc08b143372)



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
├── src
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
- The .cpp files manipulate the member variables, usually by applying algorithms and/or logic of varying complexity
- **The test folder is intended for PlatformIO Unit Testing and project tests & the lib folder is intended to be used for project specific (private) libraries, neither is being used as of now
- **main.cpp:**
   - Instantiates timing, PID, motor control, encoder, speed, error detection and command source detection parameters
   - Setup communication channels, enable motors and motor directions
   - Done in a loop:
       - Check if connected to Bluetooth or USB
       - Get velocity command, ensure it is uncorropted, if it is throw it out
       - Acquire current wheel velocity from the encoders, use this the the velocity command to calculate the next control signal
       - Apply control signal to the motor drivers
       - Send current linear and rotational velocity values to command source for printing

![FlowControl1](https://github.com/GhanGhan/Robot_Smart_Car/assets/17633599/f74f8aac-6eee-4612-aaa9-56bf0fbc4456)
***Figure 5: Flow Control Part 1***

![FlowControl2](https://github.com/GhanGhan/Robot_Smart_Car/assets/17633599/f5a37d6a-2597-468d-8ee2-eebee93cd685)
***Figure 5: Flow Control Part 2***

- **platformio.ini:**
   - The configuration file that lets you set up your development environment, share your code and configurations with others, and leverage a declarative approach to development
   - Can use platformio.ini to configure multiple platforms and architectures without having to worry about different toolchains or build systems for each one.

# Installation
- This project was primarily made using Visual Studio code and PlatformIO, to download both follow this guide from the [DroneBotWorkshop](https://dronebotworkshop.com/platformio/)
- Can also use the [Arduino IDE 2](https://docs.arduino.cc/software/ide-v2), follow this guide for dowloading and getting started
    - This was useful for Serial plotting and debugging
 
# License
- [![License: CC BY 3.0](https://img.shields.io/badge/License-CC%20BY%203.0-lightgrey.svg)](https://creativecommons.org/licenses/by/3.0/)
- This project is licensed under the [Creative Commons Attribution 3.0 Unported License](https://creativecommons.org/licenses/by/3.0/).
  
# Acknoledgments
- I would like to thank the DroneBotWorkshop for notifying me about PlatformIO and helping me set it up

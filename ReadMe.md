# Overview
The robot car is a two-wheel differential-drive mobile robot.  The wheels are actuated by DC motors whose speed and direction are sensed using quadrature encoders.  The linear and angular velocity commands for the car can be sent over USB or Bluetooth.  An associated phone app was created that allows for remote control of the car via Bluetooth.  In order to quickly get to and maintain the desired linear and angular velocities a PID control system was implemented.  An Arduino Mega is used as the microcontroller for the car.

# Table of Contents
1. Summary of Functionality
2. Repository Structure
3. Tech Stack
4. Process Description
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


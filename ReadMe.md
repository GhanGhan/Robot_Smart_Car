Summary
The robot car is a two-wheel differential-drive mobile robot.  The wheels are actuated by TTL DC motors whose speed and direction are sensed using quadrature encoders.  The linear and angular speeds of the car can be sent over USB from a pc or over Bluetooth using the associated phone app.  In order to quickly get to and maintain the desired linear and angular speeds a PID control system was implemented.  An Arduino Mega is used as the microcontroller for the car.
Features
•	Hardware switch to change speed command source from associated phone app and pc with indicator lights
•	Maximum linear speed set to 130 rpm, given wheel radius that is 44.24 cm/s
•	Maximum rotation speed set to 200 deg/s
•	Can see actual linear and angular velocity of the robot on the app
•	App sends velocity commands with start and stop bits so that erroneous commands are ignored



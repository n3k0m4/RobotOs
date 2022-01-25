## Welcome to Team 5 OS's project

Hello! This repository was made by TEAM 5 in the OS course at EURECOM 2022, and was composed by [@n3k0m4](https://github.com/n3k0m4), [@faroukfaiz10](https://github.com/faroukfaiz10), [@smlmz](https://github.com/smlmz), and was supervised by Mr ludovic Apvrille [@ludovicapvrille](https://github.com/ludovicapvrille).

### Introduction

The Eurecom Kart project consists of building a robot using the EV3 Brick, to play a game where it has win a race against other teams' robots while throwing and escaping obstacles. The programming side of this project was done thanks to low level techniques acquired during the OS lectures.

### Robot build

Our initial decision was to try and make the robot in the most minimalist and compact build possible. Even though some fancy and complex builds, with arms and sophisticated add-ons (lego wheels to the sides to limit the crashes with the walls) are tempting, and could probably be useful to keep the robot from flipping, they create a non-uniform mass distribution and make it very hard to keep a steady, high precision, and fast movement. So this choice was taken to have the minimal components mounted (i.e. optimal mass distribution), which hopefully save us from a lot of troubles.
 
![img](link_here)

Our build consists of 3 tacho motors, two mounted directly as a base in the lower part, to animate the wheels, and a third to animate the lever and throw obstacles. A sonar was also on the lower part of the robot (mainly for distance measures), and a touch sensor mounted on the front part, be used to detect when the robot crashes directly into the wall to calibrate and then continue. We also base a big part of our strategy around a gyroscope. The gyroscope is mounted in the back of the robot and will be used to calibrate the movement of the robot, detect if an accident happened, and mainly provide high precision turns.


### Components and limitations

The build is divided into 4 components apart from the ev3 brick.

#### Motors

The motor section in the ev3 documentation was very detailed and explicative, which made writing the section code about the movement an easy and straight forward task. Our architecture used two main tacho motors to move the robot around. In an ideal setting the motors will be calibrated and the speed would match between the two motors. Unfortunately, this was not the case with the package we got, so we spent and important part of our project on calibrating the movement.

**Calibration**

To do so, we first put the robot on the track and let it move freely but keep the logic of the turns and calibration, and gather the speeds from both motors during the whole trials. 




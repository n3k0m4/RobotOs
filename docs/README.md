
# Welcome to Team 5 OS's project
 
Hello! This repository was made by TEAM 5 in the OS course at EURECOM 2022, and was composed by [@n3k0m4](https://github.com/n3k0m4), [@faroukfaiz10](https://github.com/faroukfaiz10), [@smlmz](https://github.com/smlmz), and was supervised by Mr ludovic Apvrille [@ludovicapvrille](https://github.com/ludovicapvrille).
 
## Introduction
 
The Eurecom Kart project consists of building a robot using the EV3 Brick, to play a game where it has to win a race against other teams' robots while throwing and escaping obstacles. The programming side of this project was done thanks to low level techniques acquired during the OS lectures.
 
## Robot build
 
Our initial decision was to try and make the robot in the most minimalist and compact build possible. Even though some fancy and complex builds, with arms and sophisticated add-ons (Lego wheels to the sides to limit the crashes with the walls) are tempting, and could probably be useful to keep the robot from flipping, they create a non-uniform mass distribution and make it very hard to keep a steady, high precision, and fast movement. So this choice was taken to have the minimal components mounted (i.e. optimal mass distribution), which hopefully save us from a lot of troubles.

![img](link_here)
 
Our build consists of 3 tacho motors, two mounted directly as a base in the lower part, to animate the wheels, and a third to animate the lever and throw obstacles. A sonar was also on the lower part of the robot (mainly for distance measures), and a touch sensor mounted on the front part, to be used to detect when the robot crashes directly into the wall to calibrate and then continue. We also base a big part of our strategy around a gyroscope. The gyroscope is mounted in the back of the robot and will be used to calibrate the movement of the robot, detect if an accident happened, and mainly provide high precision turns.
 
 
## Components and limitations
 
The build is divided into 4 components, apart from the ev3 brick.
 
### Motors and calibrations
 
The motor section in the ev3 documentation was very detailed, which made writing the section code about the movement an easy and straight forward task. Our architecture used two main tacho motors to move the robot around. In an ideal setting, the motors will be calibrated, and the speed would match between the two motors. Unfortunately, this was not the case with the package we got, so we spent an important part of our project on calibrating the movement.
 
**Calibration:**
 
To do so, we first put the robot (fully charged) on the track and let it move freely with speed set at maximum speed but keep the logic of the turns and calibration, and gather the speeds from both motors during the whole trials.
 
![img](link_here)
 
In the first figure we can see that in the grand scheme of the course, the right motor plot is almost always on top of the left motor one besides a small part when they are in sync. 
 
![img](link_here)

To be able to notice this difference, we can limit the plot interval (zoom into a small section).

![img](link_here)
 
It is, as we noticed in the test, the right motor runs a bit faster than the left one, and the accumulation of the small deviations result in a big difference in the robot movement and behaviour.
 
To solve this issue, we need to find a coefficient that overall calibrates the difference between the two tacho motors. We also use the data from before to plot the graph of the factor `left_motor_speed/right_motor_speed`, to get a sense of the difference between the two motors.

![img](link_here)
 
A factor of `value` seems to be constant thought out the movement sections of the robot, we are also aware that these values would be different on other settings of the robot (less speed or more speed, and less charged battery).
 
The differences between the wheels above is maximal because we're using maximum speed. What explains the relatively big factor around 3%-5% is that the right motor can reach higher speeds than the left one. Asking both motors to run at their max speed maximizes the difference between the two. Noticing this gives us the first solution to our problem: Running at a lower speed than the max speed. We noticed that both motors can easily reach a speed of 800 if fully charged. We decided to go for that value for the rest of the tests.

Unfortunately, that was not enough to fix the speed difference between the motors, as there were errors that were accumulating after running them for a relatively longer period of time (>60s). We decided to correct those differences programmatically by making use of the gyroscope and making sure the robot follows a defined angle. We corrected the speeds of the motors with a factor relative to the angle difference. We will dive deeper into this in the implementation section.

To sum it up: To fix the motor's speed difference, two solutions were combined:
- Solution 1: Run at a speed both motors can reach (i.e. 800)
- Solution 2: Programmatically ensure the robot follows the same angle when moving forward.

Finally, the third motor doesn't need any calibration, as its role is to only move down to throw the obstacle and up to avoid obstructing the touch sensor.
 
### Gyroscope
 
The use of the gyroscope is limited to the detection of the angles. We also based the movement correction (keep a straight line movement) with the use of the original angle to keep in case of any accident with other robots.

Since the gyroscope plays an important role (Probably second only to motors), as it intervened in correcting the motor's speed difference, making turns and recovering from accidents to name a few, it's no surprise that we required its value to be as accurate as possible. 
 
**Calibration:**

There are 2 main issues when it comes to the gyroscope: Drift and lag.

- Drift: Drift is when the values of the gyro change while it's still (i.e. not turning). We noticed that this problem showed up after a number of laps/turns. Inside the sensor section in the ev3 block there is a mode for the gyroscope called `GYRO_CAL_MODE`, this mode is used to stabilize the values of the sensor once switched on. Setting the gyro mode to `GYRO_CAL_MODE` and back to `GYRO_ANG_MODE` after a small wait fixes this issue.
- Lag: We didn't have that big of an issue when it came to lag, as we weren't turning at a big rate. However we still decided to handle it to reduce potential errors down the line. To fix this issue, we reduced our motors speed when turning as we got closer to the desired angle, this made sure that the angle rate is not important once we're close to stopping. We noticed that our robot rotation got much more accurate after this, especially on the long term (i.e. after many turns).
 

 
### Sonar
 
The sonar implementation was not a difficult part, as our goal was to keep looking in a direct line (No shapes detection) and measure the distance to the next obstacle (walls included).
 
**Calibration:**
 
One big issue we faced with the sonar values was that the buffers we get for the sensor getting moving values (big oscillation in the values) and not being able to update correctly. To remediate this issue, we implemented `get_stable_sonar_value` in `sensors.c`, that when detecting many noise in the values, it stops and loops until it's able to get two consecutive values with a maximum threshold of 2 cm difference. See [code](https://github.com/n3k0m4/RobotOs/tree/website#calibrating-sonar)

### Touch sensor

The touch sensor is the only sensor that is analog and provides correct values. Its job is to detect when the robot in pushing a wall so we can recalibrate and continue the course. No calibration was needed for this sensor.

## Implementation

The implementation of all the robot's logic is wrapped around the ev3dev library. We have decided not to complicate our code for the readers and write the necessary wrappers around the native method of `ev3dev-c`. This wrapper methods and the modular architecture (next section) make it easy for anyone to follow the code and understand the use of each method.

### Architecture and tree

Our idea starting this project was to make a modular type architecture (usual for C projects), where we have our source code (our modules) in the `source` directory, an `include` directory (contains the headers of the modules; equivalent of interfaces), a `build` directory to put the compiled `.o` files before linking them, and a `libraries` directory when we put the necessary `.a` files to statically link them in our `main` binary.    

Inside `ev3dev-c` we have our submodule that we build our project with. The project's tree looks like the following:
```
.
├── build
│   ├── main.o
│   ├── movement.o
│   ├── sensors.o
│   └── strats.o
├── ev3dev-c  [20 entries exceeds filelimit, not opening dir]
├── include
│   ├── logger.h
│   ├── movement.h
│   ├── sensors.h
│   ├── strats.h
│   └── utils.h
├── libraries
│   └── libev3dev-c.a
├── Makefile
├── README.md
└── source
    ├── main.c
    ├── movement.c
    ├── sensors.c
    └── strats.c

```
For mode details on how to compile the code look [here](https://github.com/n3k0m4/RobotOs/tree/main#welcome-to-team-5-oss-project).

### Movement and Sensors

In this section we go a bit in the details of the methods and the logic implemented in the `sensors` and `movement`.

#### Movement
 
The interface to movement looks like the following
```C
#ifndef MOVEMENT_H
#define MOVEMENT_H

#include <stdio.h>
#include <unistd.h>
#include "ev3.h"
#include "ev3_port.h"
#include "ev3_tacho.h"

#define PORT_LEFT OUTPUT_B
#define PORT_RIGHT OUTPUT_C
#define PORT_OBSTACLE OUTPUT_A
#define ANGLE_THRESHOLD 10 // Gyroscope angle threshold
#define FIRST_TURN_THRESHOLD 300 // Starting section threshold
#define LONG_PART_THRESHOLD 800 // Long hall section threshold
#define SHORT_PART_THRESHOLD 600 // Small hall section threshold

void init_movement(); // Init tacho motors & set sn_left, sn_right
void move(int speed); // Move with both motors using speed (not calibrated)
void move_keeping_angle(int angle, int speed); // Move with calibration using a fix angle at start 
void stop(uint8_t command); // Stop motors
void run_right_motor_only(int speed); // Turn right motor only
void run_left_motor_only(int speed); // Turn left motor only
void turn_to_angle(int destination_angle, int thres); // Turn the robot to an angle, this method uses calibration to get the correct angle.
void get_left_motor_position(int *position); // Gets the position of the left motor in Tacho counts
void get_right_motor_position(int *position); // Gets the position of the left motor in Tacho counts

#endif
```

This file exports the declared methods to other files that includes it (mainly `strats.h`). In this file, we define first the `PORTS` global constants to show which motor is linked to which output in the ev3 brick (useful is case we want to swipe cables), we also define the `THRESHOLD` global constants useful for the robot login anc behavior.

The methods in the `movement.c` are mainly wrappers around ev3dev kit, so we won't show any details on them in this section, you can find the code [here](https://github.com/n3k0m4/RobotOs). 

The two interesting methods are `move_keeping_angle` and `turn_to_angle`, as they use some calibration to keep a good movement.

```C
void move_keeping_angle(int angle, int speed)
{
    int current_angle;
    get_gyro_value(&current_angle);
    int deviation = abs(current_angle - angle);
    int reduced_speed = speed * (1 - (float)deviation / 90);
    reduced_speed = _validate_speed(reduced_speed);
    if (current_angle > angle)
    {
        _run_motor_forever(sn_motor_right, speed);
        _run_motor_forever(sn_motor_left, reduced_speed);
    }
    else
    {
        _run_motor_forever(sn_motor_right, reduced_speed);
        _run_motor_forever(sn_motor_left, speed);
    }
}

void turn_to_angle(int destination_angle, int thres)
{
    stop(TACHO_COAST);
    int current_angle;
    get_gyro_value(&current_angle);
    int angle_to_turn = (destination_angle - current_angle) % 360;
    uint8_t sn_motor = angle_to_turn >= 0 ? sn_motor_left : sn_motor_right;
    uint8_t sn_other_motor = sn_motor == sn_motor_right ? sn_motor_left : sn_motor_right;
    int speed = max_speed;
    _run_motor_forever(sn_motor, speed);
    _run_motor_forever(sn_other_motor, -speed);
    while (abs(angle_to_turn) > thres)
    {
        get_gyro_value(&current_angle);
        angle_to_turn = (destination_angle - current_angle) % 360;
        speed = (int)(max_speed * ((float)abs(angle_to_turn) / 180));
        _run_motor_forever(sn_motor, speed);
        _run_motor_forever(sn_other_motor, -speed);
    }
    stop(TACHO_HOLD);
}
```


### Calibration methods

#### Calibrating Sonar
``` C
int get_stable_sonar_value(int *value_buf){
    int previous_sonar_value;
    get_sonar_value(&previous_sonar_value);
    const int ERROR_THRESHOLD = 20; // 2cm
    int sonar_value;
    int nb_constant_measures = 0;
    while (nb_constant_measures < 2){
        SLEEP(500);
        get_sonar_value(&sonar_value);
        printf("Getting stable sonar value ...\n");
        if (abs(previous_sonar_value - sonar_value) < ERROR_THRESHOLD) nb_constant_measures++;
        else nb_constant_measures = 0;
        previous_sonar_value = sonar_value;
    }
    *value_buf = sonar_value;
}
```


#### Calibrating gyroscope
``` C
void calibrate_gyro(){
    printf("Calibrating gyro ...\n");
    set_sensor_mode(sn_gyro, GYRO_CAL_MODE);
    SLEEP(1000);
    set_sensor_mode(sn_gyro, GYRO_ANG_MODE);
    SLEEP(500);
}
```
## Strategies

## Test



## Dump
(i.e. No correction if angle difference is 0, and setting one motor speed to 0 if angle is 90°)

To calibrate the gyroscope, we could switch this mode on before every test, or implement a method that switches between modes when required.

We have decided to implement in our `sensors.c` a utility function `calibrate_gyro` that switches to calibrate mode, sleep for a few seconds to let the calibration take action and then switches back to the angular mode `GYRO_ANG_MODE`. See [code](https://github.com/n3k0m4/RobotOs/tree/website#calibrating-gyroscope)

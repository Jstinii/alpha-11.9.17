#ifndef _BASICS_H_
#define _BASICS_H_

int autoMode; // autoMode for selection of mutiples autons

#define LEFT_IME 0 // macro for left IME
#define RIGHT_IME 1 // macro for right IME
Gyro gyro; // gyro object

void drive(int left, int right);

#define GOAL_LIFT 1 // macro to access mogo lift in lift() function
#define CONE_LIFT 2 // macro to access elevator lift in lift() function

Encoder liftEncoder; // encoder object
int liftHeight; // variable to store elevator height
#define LIFT_LIMIT 3 // macro for for elevator limit switch port
#define GOAL_LEFT 1 // mogo lift left pot
#define GOAL_RIGHT 3 // mogo lift right pot

void lift(int lift, int power); // lift combo function

void shifter(int power); // chainbar function

void intake(int power); // cone intake function

#define JOY_STICK 0 // macro for accessing values from sticks in joystick() function
#define MAIN_CONTROL 1 // address of main controller (controls drive)
#define AUX_CONTROL 2 // address of aux controller (controls stacking)

int joystick(int controller, int channel, int button); // joystick value combo function

#endif // _BASICS_H_

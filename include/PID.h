#ifndef _PID_H_
#define _PID_H_

int imeLeft; // left IME value
int imeRight; // right IME value
int leftError; // left IME error from position
int rightError; // right IME error from position
float liftPowerLeft; // power returned by PID function for left lift
float liftPowerRight; // power returned by PID function for right lift
float drivePowerLeft; // power returned by PID function for left side
float drivePowerRight; // power returned by PID function for right drive
float optimalVoltage; // desired/simulated voltage
float batteryScale; // battery scale variabl

void resetIMEs(); // function to reset IMEs
void updateDrive(int goal); // drive PID
void updateLift(int goal, int max); // mogo lift PID
void updateTurn(int goal, int max); // PID for turning

#endif // _PID_H_

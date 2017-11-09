#include "main.h"
#include "basics.h"

void resetIMEs(){ // resets IME and update IME variables
  imeReset(0);
  imeGet(0, &imeLeft);

  imeReset(1);
  imeGet(1, &imeRight);

  drivePowerLeft = drivePowerRight = 0; // set drivePowers to 0
}

void updateDrive(int goal, int max){ // PID control for drivetrain (only P currently)

  // get IME values and store
  imeGet(0, &imeLeft);
  imeGet(1, &imeRight);

  // calulates the error for each side
  leftError = goal - imeLeft;
  rightError = goal + imeRight;

  // calulates the drivePower for each side
  drivePowerLeft = .4 * leftError * batteryScale;
  drivePowerRight = .4 * rightError * batteryScale;
}

void updateLift(int goal, int max){ // PID control for mogo lift (only P currently)

  // calculates power for each side separately using potentiometer values
  // (goal - analogRead) is the error
  liftPowerLeft = .15 * (goal - analogRead(GOAL_LEFT)) * batteryScale;
  liftPowerRight = .15 * (goal - analogRead(GOAL_RIGHT)) * batteryScale;

}

void updateTurn(int goal){ // PID control for turning (only P currently)

  leftError = rightError = goal - gyroGet(gyro); // gets errors from gyro value

  // calculates power for each side (assuming symmetrical turn)
  drivePowerLeft = -1.2 * leftError * batteryScale;
  drivePowerRight = 1.2 * rightError * batteryScale;
}

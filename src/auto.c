/** @file auto.c
 * @brief File for autonomous code
 *
 * This file should contain the user autonomous() function and any functions related to it.
 *
 * Any copyright is dedicated to the Public Domain.
 * http://creativecommons.org/publicdomain/zero/1.0/
 *
 * PROS contains FreeRTOS (http://www.freertos.org) whose source code may be
 * obtained from http://sourceforge.net/projects/freertos/files/ or on request.
 */

#include "main.h"

/*
 * Runs the user autonomous code. This function will be started in its own task with the default
 * priority and stack size whenever the robot is enabled via the Field Management System or the
 * VEX Competition Switch in the autonomous mode. If the robot is disabled or communications is
 * lost, the autonomous task will be stopped by the kernel. Re-enabling the robot will restart
 * the task, not re-start it from where it left off.
 *
 * Code running in the autonomous task cannot access information from the VEX Joystick. However,
 * the autonomous function can be invoked from another task if a VEX Competition Switch is not
 * available, and it can access joystick information if called in this way.
 *
 * The autonomous task may exit, unlike operatorControl() which should never exit. If it does
 * so, the robot will await a switch to another mode or disable/enable cycle.
 */

void autonomous() {
  gyroReset(gyro);
  resetIMEs();

  while(analogRead(GOAL_LEFT) < 3150){ // move the goal to intake position
    updateLift(3150);
    motorSet(2, -liftPowerLeft);
    motorSet(3, liftPowerRight);
  }
  lift(GOAL_LIFT, 0); // set liftPower to 0 if wierd case occurs

  resetIMEs(); // reset IMEs
  while(imeLeft < 1700){ //move while lowering lift
    updateDrive(1700, 0);
    drive(drivePowerLeft, drivePowerRight);
  }

  delay(750); // stop to let interia play out

  while(analogRead(GOAL_LEFT) > 1500){ //intake goal
    updateLift(1500);
    motorSet(2, -liftPowerLeft);
    motorSet(3, liftPowerRight);
  }
  lift(GOAL_LIFT, 0); // set liftPower to 0 if wierd case occurs

  //stack cone

  resetIMEs();
  while(imeLeft > -1200){ // drive back to the zones
    updateDrive(-1200, 0);
    drive(drivePowerLeft, drivePowerRight);
  }

  while(gyroGet(gyro) < 135){ // turn to be parallel with 10 pt bar
    updateTurn(135 , 0);
    drive(drivePowerLeft/2, drivePowerRight);
  }

  while(gyroGet(gyro) < 225){ // turn to face the 10pt bar
    updateTurn(225, 0);
    drive(0, drivePowerRight);
  }

}

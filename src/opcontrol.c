/** @file opcontrol.c
 * @brief File for operator control code
 *
 * This file should contain the user operatorControl() function and any functions related to it.
 *
 * Any copyright is dedicated to the Public Domain.
 * http://creativecommons.org/publicdomain/zero/1.0/
 *
 * PROS contains FreeRTOS (http://www.freertos.org) whose source code may be
 * obtained from http://sourceforge.net/projects/freertos/files/ or on request.
 */

#include "main.h"

void driveTask(void * parameter){ // separate task for drivetrain control

    int drivePower, driveTurn; // variables for arcade drive
    int driveMode = 0; // mode that determined which side of robot is 'front'
    int goalPower; // variable for mogo lift power

    while (1 == 1){ // always loop

      if (joystick(MAIN_CONTROL, 7, JOY_UP)){ // mogo is front
        driveMode = 0;
      }
      if (joystick(MAIN_CONTROL, 7, JOY_DOWN)){ // cone intake is front
        driveMode = 1;
      }

      // mobile goal control
      goalPower = joystick(MAIN_CONTROL, 2, JOY_STICK);
      lift(GOAL_LIFT, goalPower);


      if (driveMode == 0){ // drive control when mogo is front
    		drivePower = joystick(MAIN_CONTROL, 3, JOY_STICK);
    		driveTurn = joystick(MAIN_CONTROL, 4, JOY_STICK);
    		drive(drivePower + driveTurn, drivePower - driveTurn);
      }
      else if (driveMode == 1){ // drive control when cone intake is front
        drivePower = -joystick(MAIN_CONTROL, 3, JOY_STICK);
        driveTurn = -joystick(MAIN_CONTROL, 4, JOY_STICK);
        drive(drivePower - driveTurn, drivePower + driveTurn);
      }
    }

	delay(10); // prevent hogging from main/stacking task
}

/*
 * Runs the user operator control code. This function will be started in its own task with the
 * default priority and stack size whenever the robot is enabled via the Field Management System
 * or the VEX Competition Switch in the operator control mode. If the robot is disabled or
 * communications is lost, the operator control task will be stopped by the kernel. Re-enabling
 * the robot will restart the task, not resume it from where it left off.
 *
 * If no VEX Competition Switch or Field Management system is plugged in, the VEX Cortex will
 * run the operator control task. Be warned that this will also occur if the VEX Cortex is
 * tethered directly to a computer via the USB A to A cable without any VEX Joystick attached.
 *
 * Code running in this task can take almost any action, as the VEX Joystick is available and
 * the scheduler is operational. However, proper use of delay() or taskDelayUntil() is highly
 * recommended to give other tasks (including system tasks such as updating LCDs) time to run.
 *
 * This task should never exit; it should end with some kind of infinite loop, even if empty.
 */
void operatorControl() {

taskCreate(driveTask, TASK_DEFAULT_STACK_SIZE, NULL, TASK_PRIORITY_DEFAULT);
// creates driver task
bool stack = true; // default auto stack on

	while (true) { // always looping

    if (joystick(AUX_CONTROL, 7, JOY_LEFT)){ // turn off autostack
      stack = false;
    }
    if (joystick(AUX_CONTROL, 7, JOY_RIGHT)){ // turn on autostack
       stack = true;
    }

    if (digitalRead(LIFT_LIMIT) == LOW){ // reset encoder it swtich is depressed
      encoderReset(liftEncoder);
    }
    liftHeight = encoderGet(liftEncoder); // update value of liftHeight with encoder value
    if (joystick(AUX_CONTROL, 5, JOY_UP)){ // move elevator to pickup position (stage 1)
      while (liftHeight < 270){
        lift(CONE_LIFT, -127);
        liftHeight = encoderGet(liftEncoder);
      }
      while (liftHeight > 300){
        lift(CONE_LIFT, 64);
        liftHeight = encoderGet(liftEncoder);
      }
    }
    else if ((joystick(AUX_CONTROL, 5, JOY_DOWN)) && (liftHeight >= 0)){
      //move elevator down while intaking cone
      while ((liftHeight > 20) && (digitalRead(LIFT_LIMIT) == HIGH)){
      lift(CONE_LIFT, 127);
      intake(96);
      liftHeight = encoderGet(liftEncoder);
      }
      intake(0);
    }
    else if (joystick(AUX_CONTROL, 8, JOY_RIGHT)){ // move to elevator stage 2
      while (liftHeight < 600){
        lift(CONE_LIFT, -127);
        liftHeight = encoderGet(liftEncoder);
      }
      while (liftHeight > 650){
        lift(CONE_LIFT, 64);
        liftHeight = encoderGet(liftEncoder);
      }
      lift(CONE_LIFT, 0);
    }
    else if (joystick(AUX_CONTROL, 8, JOY_LEFT)){ // move to elevator stage 0
      while ((liftHeight > 20) && (digitalRead(LIFT_LIMIT) == HIGH)){
        lift(CONE_LIFT, 127);
        liftHeight = encoderGet(liftEncoder);
      }
    }
    else{
      lift(CONE_LIFT, 0); // if not buttons pressed, don't move elevator
    }

    if (joystick(AUX_CONTROL, 6, JOY_UP)){ // stack cone onto stack
      if(stack){
        shifter(127);
        intake(12);
        delay(1000 * (1/batteryScale));
      }
      else{
        shifter(127);
      }
    }
    else if (joystick(AUX_CONTROL, 6, JOY_DOWN)){
      // release cone and bring chainbar to intake position
      if(stack){
        shifter(-127);
        intake(-127);
        delay(1000 * (1/batteryScale));
      }
      else{
        shifter(-127);
      }
    }
    else{
      shifter(0); // if nothing pressed, don't move chainbar or intake
      intake(0);
    }

		delay(10); // delay to prevent hogging from drivetrain task
	}
}

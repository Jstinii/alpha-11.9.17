/** @file init.c
 * @brief File for initialization code
 *
 * This file should contain the user initialize() function and any functions related to it.
 *
 * Any copyright is dedicated to the Public Domain.
 * http://creativecommons.org/publicdomain/zero/1.0/
 *
 * PROS contains FreeRTOS (http://www.freertos.org) whose source code may be
 * obtained from http://sourceforge.net/projects/freertos/files/ or on request.
 */

#include "main.h"

/*
 * Runs pre-initialization code. This function will be started in kernel mode one time while the
 * VEX Cortex is starting up. As the scheduler is still paused, most API functions will fail.
 *
 * The purpose of this function is solely to set the default pin modes (pinMode()) and port
 * states (digitalWrite()) of limit switches, push buttons, and solenoids. It can also safely
 * configure a UART port (usartOpen()) but cannot set up an LCD (lcdInit()).
 */
void initializeIO() {
}

/*
 * Runs user initialization code. This function will be started in its own task with the default
 * priority and stack size once when the robot is starting up. It is possible that the VEXnet
 * communication link may not be fully established at this time, so reading from the VEX
 * Joystick may fail.
 *
 * This function should initialize most sensors (gyro, encoders, ultrasonics), LCDs, global
 * variables, and IMEs.
 *
 * This function must exit relatively promptly, or the operatorControl() and autonomous() tasks
 * will not start. An autonomous mode selection menu like the pre_auton() in other environments
 * can be implemented in this task if desired.
 */
void initialize() {
  autoMode = 2; // default auton
  liftEncoder = encoderInit(1, 2, false); // init optical encoder on elevator
  gyro = gyroInit(2, 0); // init gyro for turns
  pinMode(LIFT_LIMIT, INPUT); // init limit switch on elevator
  imeInitializeAll(); // init all imes

  optimalVoltage = 8000.00; // optimal(simulated) battery voltage
  if(powerLevelMain() != 0){ // make sure no strange value
    batteryScale = optimalVoltage/powerLevelMain(); // scale voltage
  }
  else{
   batteryScale = 1; // scale to 1 if powerLevelMain() returns bad value
  }

  lcdInit(uart2); // init LCD
  lcdClear(uart2); // clear LCD just in case

  lcdSetText(uart2, 2, "<     Enter    >"); // bottom select text
  while((lcdReadButtons(uart2) != 2) && !(isEnabled())){ // while middle not pressed
    // and robot isn't enabled

    switch(autoMode){ // swtich based on autoMode value
      case 1:
        lcdSetText(uart2, 1, "    Nothing    "); // if 1, no auton
      break;
      case 2:
        lcdSetText(uart2, 1, "   20pt Right  "); // if 2, 20pt from right side
      break;
      case 3:
        lcdSetText(uart2, 1, "   20pt Left   "); // if 3, 20pt from left side
      break;
    }

    if(lcdReadButtons(uart2) == 1){ // if left button
      autoMode = autoMode - 1; // subtract one from autoMode value
      if (autoMode < 1){ // cycle
        autoMode = 3;
      }
    }
    else if (lcdReadButtons(uart2) == 4){ //if right button
      autoMode = autoMode + 1; // add one to autoMode value
      if (autoMode > 3){ // cycle
        autoMode = 1;
      }
    }

    delay(100); // delay to prevent hogging

  }

  lcdClear(uart2); // clear for stablity
  delay(100); // prevent task hogging glitch
  lcdPrint(uart2, 1, "Main Vol: %d\n", powerLevelMain()); // display main battery voltage
  lcdPrint(uart2, 2, "Scale: %f\n", batteryScale); // display scale to check for extraneous value
  delay(1000); // delay to end init
}

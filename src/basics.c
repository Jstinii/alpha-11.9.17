#include "main.h"
#include "basics.h"

void drive(int left, int right){
  motorSet(4, left); //drive left port 4
  motorSet(5, -right); //drive right port 5 (reversed)
}

void lift(int lift, int power){ // combo function for both lifts
  if (lift == 1){ // mogo lift
    motorSet(2, -power); // left side port 2
    motorSet(3, power); // right side port 3
  }
  else if (lift == 2){ // elevator lift
    motorSet(7, power);
    motorSet(8, -power);
  }
}

void shifter(int power){ // function for moving chainbar
  motorSet(9, power); // port 9
}

void intake(int power){ // function of controlling cone intake
  motorSet(6, -power); // port 6
}

int joystick(int controller, int channel, int button){
  //combo joystick function for simplification
  if (channel <= 4){ // if analog channel
    return joystickGetAnalog(controller, channel);
  }
  else if (channel >= 5){ // if digital channel
    return joystickGetDigital(controller, channel, button);
  }
  return 0; // something goes wrong return 0;
}

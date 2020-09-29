#ifndef HCC_H
#define HCC_H
#include <Arduino.h>
#include "pin_define.h"
#include "enums.h"
void control_motor(e_motor_control choice, int pwm = 255);
#endif

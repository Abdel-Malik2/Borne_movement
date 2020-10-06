#ifndef HCC_H
#define HCC_H
#include <Arduino.h>
#include "pin_define_types.h"
#include "enums.h"
void controlMotor(e_motor_control choice, int pwm = 255);
#endif

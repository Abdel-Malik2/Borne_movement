#ifndef HCC_H
# define HCC_H

# include <Arduino.h>
# include "pin_define_types.h"

extern e_state          state;
extern e_screen_pos     screen_pos;
extern e_motor_control  order;

void initialise_pinmode();
void control_motor(e_motor_control choice, int pwm = 255);

#endif

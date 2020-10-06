#ifndef DEBUG_PRINTING_H
# define DEBUG_PRINTING_H

# include <Arduino.h>
# include "pin_define_types.h"
#include "sensors.h"
# include "hydraulic_cylinder_control.h"
# include "check_presence.h"

void printDistance(uint16_t distance_down);
void printScreenPos(e_screen_pos screen_pos);
void printOrder(e_motor_control order);
void printState(void);

#endif
#ifndef SWITCH_STATE_H
# define SWITCH_STATE_H

# include <Arduino.h>
# include "pin_define_types.h"
# include "sensors.h"
# include "debug_printing.h"
# include "screen_timer.h"
# include "hydraulic_cylinder_control.h"
# include "check_presence.h"

void computeState(void);
#endif
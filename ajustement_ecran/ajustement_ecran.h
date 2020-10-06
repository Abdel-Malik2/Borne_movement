#ifndef AJUSTEMENT_ECRAN_H
# define AJUSTEMENT_ECRAN_H

# include <Arduino.h>
# include <Wire.h> //Native librairy
# include <VL53L0X.h> //Pololu library
# include <neotimer.h> // //Joe Rullan library with modification: Move definitions to cpp file

# include "hydraulic_cylinder_control.h"
# include "debug_printing.h"
# include "pin_define_types.h"
# include "sensors.h"
# include "screen_timer.h"
# include "switch_state.h"
# include "check_presence.h"

e_state         state = screen_reset;
e_screen_pos    screen_pos = unknown;
e_motor_control order = STOP;

/** Timer move screen **/
bool            presence = false;

VL53L0X         sensor_up = VL53L0X();
VL53L0X         sensor_down = VL53L0X();

/** Timer move screen **/
Neotimer        timeout_move_up = Neotimer(11000);
Neotimer        timeout_move_down = Neotimer(5000);

/** Timer detect presence **/
Neotimer        timeout_presence_in = Neotimer(2500);
Neotimer        timeout_presence_out = Neotimer(1500);

/** Timer max fiting **/
Neotimer        timeout_fiting_up = Neotimer(5000);
Neotimer        timeout_fiting_down = Neotimer(11000);

#endif

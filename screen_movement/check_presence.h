#ifndef CHECK_PRESENCE_H
# define CHECK_PRESENCE_H

# include <Arduino.h>
# include <VL53L0X.h>
# include "pin_define_types.h"
# include "sensors.h"
# include "debug_printing.h"
# include "screen_timer.h"

extern bool     presence;
extern VL53L0X  sensor_up;
extern VL53L0X  sensor_down;

uint16_t getDistance(VL53L0X *sensor);
bool getAdjust(void);
void updatePresence(void);

#endif
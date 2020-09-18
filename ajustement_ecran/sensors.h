#ifndef SENSORS_H
#define SENSORS_H

#include <VL53L0X.h> //Pololu library

/** Put sensors in a 'off' or 'sleep' state. 
 *  Its required for assigning new I2C adresses individually
 */
void turn_off_sensors();

void prepareSensor(byte pin, bool stat);

/** Initialise a sensor and change its I2C address
 *  Input :   *sensor : a ptr of a sensor object 
 *            pin : The hardware pin connect to the sensor XSHUT
 *            adr : The new sensor address for I2C connection
**/
void initSensor(VL53L0X *sensor, byte pin, uint8_t adr);

#endif

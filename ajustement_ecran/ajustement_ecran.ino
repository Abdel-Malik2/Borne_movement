#include <Wire.h> //Native librairy
#include <neotimer.h> //Joe Rullan library
#include "pin_define.h"
#include "sensors.h"

VL53L0X sensor_up = VL53L0X();
VL53L0X sensor_down = VL53L0X();

void setup() {
  Serial.begin(9600);
  Wire.setSCL(SCL_Wire);
  Wire.setSDA(SDA_Wire);
  Wire.begin();
  
  turn_off_sensors();
  delay(100);
  prepareSensor(PIN_XSHUT_1, HIGH);
  initSensor(&sensor_up, PIN_XSHUT_1, ADR_SENSOR_1);
  prepareSensor(PIN_XSHUT_2, HIGH);  
  initSensor(&sensor_down, PIN_XSHUT_2, ADR_SENSOR_2);
}

void loop() {
  Serial.print("\n\nsensor_up:\t");
  Serial.print(sensor_up.readRangeContinuousMillimeters());//Serial.print(sensor_up.read());
  if (sensor_up.timeoutOccurred()) { Serial.print(" TIMEOUT"); }
  Serial.println();
  Serial.print("\nsensor_down:\t");
  Serial.print(sensor_down.readRangeContinuousMillimeters());//  Serial.print(sensor_down.read());
  if (sensor_down.timeoutOccurred()) { Serial.print(" TIMEOUT"); }
  Serial.println();
  delay(200);
}

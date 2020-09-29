#include "pin_define.h"
#include "sensors.h"

void turn_off_sensors(){
  //sensor_up
  pinMode(PIN_XSHUT_1, OUTPUT);
  digitalWrite(PIN_XSHUT_1, LOW);
  //sensor_down
  pinMode(PIN_XSHUT_2, OUTPUT);
  digitalWrite(PIN_XSHUT_2, LOW);
}

void prepareSensor(byte pin, bool stat)
{
  pinMode(pin, OUTPUT);
  digitalWrite(pin, stat);
}

void initSensor(VL53L0X *sensor, byte pin, uint8_t adr)
{
  pinMode(pin, INPUT);
  delay(200);
  Serial.println(F("Init - sensor"));
  while(!sensor->init()){
    Serial.println(F("Initialisation Failed"));
    delay(100);
  }
  Serial.println(F("Initialisation completed"));
  delay(100);
  sensor->setAddress(adr);
  sensor->setTimeout(500);
  sensor->startContinuous(50);
}

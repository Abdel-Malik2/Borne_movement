#include "pin_define.h"
#include "sensors.h"

void turn_off_sensors(){
  //sensor_up
  pinMode(PIN_XSHUT_1, OUTPUT);
  digitalWrite(PIN_XSHUT_1, LOW);
  //sensor_down
  pinMode(PIN_XSHUT_1, OUTPUT);
  digitalWrite(PIN_XSHUT_2, LOW);
}

void prepareSensor(byte pin, bool stat)
{
  pinMode(pin, OUTPUT);
  digitalWrite(pin, stat);
}

void initSensor(VL53L1X *sensor, byte pin, uint8_t adr)
{                           // commentary section is for VL53L0X
    pinMode(pin, INPUT);
    delay(200);
    Serial.print("\nINIT SENSOR");
    //   sensor->init();
    if (!sensor->init(true))
    {
        Serial.println(" ERROR");
        delay(100);
        NVIC_SystemReset();
    }
        Serial.println(" OK");

    delay(100);
    sensor->setAddress(adr);
    // sensor->setTimeout(500);
    sensor->setTimeout((uint16_t)500);
    sensor->setDistanceMode(VL53L1X::Medium);
    sensor->setMeasurementTimingBudget(33000);
    // sensor->startContinuous();
    sensor->startContinuous(50);
}

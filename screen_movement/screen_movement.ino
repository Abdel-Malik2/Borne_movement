#include "screen_movement.h"

void setup()
{
    Serial.begin(9600);
    Serial.println(F("Setup start"));
    Serial.print(F("\tInit. interrupter: "));
    attachInterrupt(digitalPinToInterrupt(interrupter), ISR, RISING);
    Serial.print(F("done\n\tInit. hydraulic actuator: "));
    initialise_pinmode();
    Serial.print(F("done\n\tInit. I2C: "));
    Wire.setSCL(SCL_Wire);
    Wire.setSDA(SDA_Wire);
    Wire.begin();
    Serial.print(F("done\n\tInit. sensor: "));
    turn_off_sensors();
    delay(100);
    prepareSensor(PIN_XSHUT_1, HIGH);
    initSensor(&sensor_up, PIN_XSHUT_1, ADR_SENSOR_1);
    prepareSensor(PIN_XSHUT_2, HIGH);
    initSensor(&sensor_down, PIN_XSHUT_2, ADR_SENSOR_2);
    initTimer();
    Serial.println(F("done\nSetup completed"));
}

void loop()
{
    computeState();
    delay(200);
}

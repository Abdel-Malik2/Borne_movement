#include "ajustement_ecran.h"

void setup()
{
    Serial.begin(9600);
    Serial.println(F("setup start"));
    attachInterrupt(digitalPinToInterrupt(interrupter), ISR, RISING);
    initialise_pinmode();
    Wire.setSCL(SCL_Wire);
    Wire.setSDA(SDA_Wire);
    Wire.begin();
    turn_off_sensors();
    delay(100);
    prepareSensor(PIN_XSHUT_1, HIGH);
    initSensor(&sensor_up, PIN_XSHUT_1, ADR_SENSOR_1);
    prepareSensor(PIN_XSHUT_2, HIGH);
    initSensor(&sensor_down, PIN_XSHUT_2, ADR_SENSOR_2);
    initTimer();
    Serial.println(F("setup completed"));
}

void loop()
{
    computeState();
    delay(200);
}

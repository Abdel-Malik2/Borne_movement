#include <Arduino.h>


#define interrupter PB7   //D4

void setup()
{
    Serial.begin(9600);
    Serial.println(F("setup start"));
    attachInterrupt(digitalPinToInterrupt(interrupter), ISR, RISING);
}

void loop()
{
    Serial.println("Loop ");
    delay(200);
}

/*
 * The method will run when the interrupter is on.
 */
void ISR()
{
    Serial.println("CALL ISR");
}

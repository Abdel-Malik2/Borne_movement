#include "debug_printing.h"

void printDistance(uint16_t distance)
{
    Serial.print(F(" sensor: "));
    if (distance != ERROR_SENSOR_TIMEOUT)
        Serial.print(distance);
    else
        Serial.print(F(" ERROR_SENSOR_TIMEOUT"));
    Serial.print(F(" "));
}

void printScreenPos(e_screen_pos screen_pos)
{
    Serial.print(F(" SCREEN POSITION => "));
    switch (screen_pos)
    {
        case unknown:
            Serial.print(F("unknown "));
            break;
        case origin:
            Serial.print(F("origin "));
            break;
        case above:
            Serial.print(F("above "));
            break;
        case under:
            Serial.print(F("under "));
            break;
        default:
            break;
    }
}

void printOrder(e_motor_control order)
{
    Serial.print(F(" ORDER => "));
    switch (order)
    {
        case UP:
            Serial.print(F("UP\t"));
            break;
        case DOWN:
            Serial.print(F("DOWN\t"));
            break;
        case STOP:
            Serial.print(F("STOP\t"));
            break;
        default:
            break;
    }
}

void printState(void)
{
    if (presence)
        Serial.print(F("presence,\t"));
    else
        Serial.print(F("!presence,\t"));
    Serial.print(F("state: "));
    switch (state)
    {
        case idle:
            Serial.println(F("Idle"));
            break;
        case fiting:
            Serial.println(F("Fiting"));
            break;
        case in_use:
            Serial.println(F("In Use"));
            break;
        case screen_reset:
            Serial.println(F("Screen Reset"));
            break;
        default:
            break;
    }
}

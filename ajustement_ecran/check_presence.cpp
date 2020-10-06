# include "check_presence.h"

uint16_t getDistance(VL53L0X *sensor)
{
  uint16_t value;

  value = sensor->readRangeContinuousMillimeters();
  if (sensor->timeoutOccurred())
    return (ERROR_SENSOR_TIMEOUT);
  return (value);
}

bool getAdjust(void) // true if a body is see by the sensor
{
  uint16_t distance_up;

    distance_up = getDistance(&sensor_up);
    printDistance(distance_up);
    Serial.println(F("(up)"));
    return (distance_up > THRESHOLDDISTANCEMIN
       && distance_up < THRESHOLDDISTANCE);
}

void updatePresence(void)
{
    uint16_t distance_down;

    distance_down = getDistance(&sensor_down);
    printDistance(distance_down);
    Serial.print(F("(down) "));
    if (distance_down > THRESHOLDDISTANCEMIN
        && distance_down < THRESHOLDDISTANCE)
    {
        Serial.print(F(" distance find, "));
        if (!timeout_presence_in.started())
        {
            timeout_presence_in.start();
            presence = false;
            Serial.print(F(" timeout_in START"));
        }
        else if (timeout_presence_in.done())
        {

            Serial.print(F(" timeout_in DONE"));
            presence = true;
        }
        else
        {
            Serial.print(F(" timeout_in CONTINU"));
            presence = false;
        }
        resetTimer(&timeout_presence_out);
    }
    else
    {
        Serial.print(F(" distance NOT find, "));
        if (!timeout_presence_out.started())
        {
            Serial.print(F("timeout_out start "));
            timeout_presence_out.start();
        }
        if (timeout_presence_out.done())
        {
          Serial.print(F("timeout_out done"));
          resetTimer(&timeout_presence_out);
          resetTimer(&timeout_presence_in);
          presence = false;
        }
        else if (timeout_presence_in.waiting())
        {
            Serial.print(F("timeout_in continu (waiting)"));
            resetTimer(&timeout_presence_in);
            // presence = true;
        }
    }
    Serial.println(F(" updatePresence finish"));

}

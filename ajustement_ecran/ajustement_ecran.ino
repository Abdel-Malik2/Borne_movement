#include <Wire.h> //Native librairy
#include <neotimer.h> //Joe Rullan library
#include "pin_define.h"
#include "enums.h"
#include "sensors.h"
#include "hydraulic_cylinder_control.h"

VL53L1X sensor_up = VL53L1X();
VL53L1X sensor_down = VL53L1X();

/** Timer **/
Neotimer timeout_move_up = Neotimer(2500);
Neotimer timeout_move_down = Neotimer(5000);
Neotimer timeout_presence_in = Neotimer(5000);
Neotimer timeout_presence_out = Neotimer(5000);
Neotimer timeout_adjust = Neotimer(2500);

e_screen_pos screen_pos = unknown;
e_state state = screen_reset;

// // presence in front of the terminal
bool adjust = false;
bool presence = false;

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

uint16_t getDistance(VL53L1X *sensor)
{
  uint16_t value;

  value = sensor->readRangeContinuousMillimeters();
  if (sensor->timeoutOccurred())
    return (ERROR_SENSOR_TIMEOUT);
  return (value);
}

void printDistance(uint16_t distance_down) //debug
{
    Serial.print("\n\nsensor :\t");
        if (distance_down != ERROR_SENSOR_TIMEOUT)
        Serial.print(distance_down);
    Serial.println();
}

bool getPresence(void) // true if somebody in front of terminal()
{
  uint16_t distance_down;

    distance_down = getDistance(&sensor_down);
    if(distance_down > THRESHOLDDISTANCEMIN)
        presence = distance_down < THRESHOLDDISTANCE;
    if (presence)
    {
        if(!timeout_presence_in.started())
            timeout_presence_in.start();
        printDistance(distance_down);// debug
        return (true);
    }
    else if (!timeout_presence_out.done())
    {
        if(!timeout_presence_out.started())
            timeout_presence_out.start();
        return (true);
    }
    return (false);
}

void resetTimer(Neotimer *timer)
{
    timer->reset();
    timer->stop();
}

void resetTimers(void)
{
    resetTimer(&timeout_presence_in);
    resetTimer(&timeout_move_up);
    resetTimer(&timeout_move_down);
    resetTimer(&timeout_presence_in);
    resetTimer(&timeout_presence_out);
    resetTimer(&timeout_adjust);
}

void caseIdle(void)
{
    control_motor(STOP);
    resetTimers();
    if (presence)
        state = person_detected;
}
void casePersonDetected(void)
{
    if (!(presence = getPresence()))
        state = screen_reset;
    else if(timeout_presence_in.done())
    {
        timeout_presence_in.reset();
        timeout_presence_in.stop();
        state = fiting;
    }
}

void caseFiting(void)
{
}

void computeState(void)
{
    presence = getPresence();
    switch (state)
    {
        case idle:
            caseIdle();
            break;
        case person_detected:
            casePersonDetected();
            break;
        case fiting:
            caseFiting();
            break;
        case screen_reset:
            // resetScreen();
            break;
        default:
            break;
    }
}

void loop() {
    computeState();
    delay(200);
}

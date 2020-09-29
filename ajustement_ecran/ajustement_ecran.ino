#include <Wire.h> //Native librairy
#include <neotimer.h> //Joe Rullan library
#include "pin_define.h"
#include "enums.h"
#include "sensors.h"
#include "hydraulic_cylinder_control.h"

VL53L0X sensor_up = VL53L0X();
VL53L0X sensor_down = VL53L0X();

/** Timer **/

Neotimer timeout_move_up = Neotimer(10000);
Neotimer timeout_move_down = Neotimer(5000);
Neotimer timeout_presence_in = Neotimer(5000);
Neotimer timeout_presence_out = Neotimer(5000);
Neotimer timeout_adjust = Neotimer(2500);

e_state state = screen_reset;
e_screen_pos screen_pos = unknown;
e_motor_control order = STOP;

// // presence in front of the terminal
bool adjust = false;
bool presence = false;

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
    if(distance_up > THRESHOLDDISTANCEMIN)
        adjust = distance_up < THRESHOLDDISTANCE;
    if (adjust)
    {
        printDistance(distance_up);// debug
        return (true);
    }
    return (false);
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
    
    printDistance(distance_down);// debug
    if(distance_down > THRESHOLDDISTANCEMIN)
        presence = distance_down < THRESHOLDDISTANCE;
    if (presence)
    {
        if(!timeout_presence_in.started())
            timeout_presence_in.start();
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
    screen_pos = origin;
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
  e_motor_control old_order = order;
  bool b = getAdjust();
  if(b)
    order = UP;
  else
    order = DOWN;
  if(old_order != STOP)
    if(old_order != order){
      order = STOP;
      state = in_use;
    }
  control_motor(order);
}

void resetScreen(){
  if(!timeout_move_up.started()){
    order = UP;
    timeout_move_up.start();
  } 
  control_motor(order);
  if(timeout_move_up.done()){
    order = DOWN;
    if(!timeout_move_up.started())
      timeout_move_down.start();
      order = DOWN;
    control_motor(order);
  }
}

void computeState(void)
{
    presence = getPresence();
    Serial.print("state = ");
    Serial.println(state);
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
            resetScreen();
            break;
        default:
            break;
    }
}

void setup() {
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
  Serial.println(F("setup completed"));
}

void loop() {
  computeState();
  delay(200);
}

/*
 * The method will run when the interrupter is on.
 */
void ISR() {
  switch(state){
    case screen_reset:
      state = idle;
     case fiting:
      if(order == UP)
        screen_pos = above;
      if(order == DOWN)
        screen_pos = under;
    break;
  }
}

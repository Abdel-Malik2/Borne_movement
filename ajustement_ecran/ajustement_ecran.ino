#include <Wire.h> //Native librairy
#include <neotimer.h> //Joe Rullan library
#include "pin_define.h"
#include "enums.h"
#include "sensors.h"
#include "hydraulic_cylinder_control.h"

VL53L0X sensor_up = VL53L0X();
VL53L0X sensor_down = VL53L0X();

/** Timer **/

Neotimer timeout_move_up = Neotimer(15000);
Neotimer timeout_move_down = Neotimer(5000);
Neotimer timeout_presence_in = Neotimer(2500);
Neotimer timeout_presence_out = Neotimer(1500);

e_state state = screen_reset;
e_screen_pos screen_pos = unknown;
e_motor_control order = STOP;

bool presence = false;

void resetTimer(Neotimer *timer)
{
    timer->reset();
    timer->stop();
}

void resetTimers(void)
{
    resetTimer(&timeout_move_up);
    resetTimer(&timeout_move_down);
}

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
  bool adjust = false;

    distance_up = getDistance(&sensor_up);
    Serial.print("GET ADJUST: sensor up distance: ");
    Serial.println(distance_up);
    if(distance_up > THRESHOLDDISTANCEMIN)
        adjust = distance_up < THRESHOLDDISTANCE;
    Serial.print(" ADJUST? : ");
    if (adjust)
    {

        // printDistance(distance_up);// debug
        Serial.print(" true! ");
        return (true);
    }
        Serial.print(" false! ");
    return (false);
}

void printDistance(uint16_t distance_down) //debug
{
    Serial.print(" sensor :\t");
        if (distance_down != ERROR_SENSOR_TIMEOUT)
        Serial.print(distance_down);
    Serial.print(" ");
}

void updatePresence(void)
{
  uint16_t distance_down;

    distance_down = getDistance(&sensor_down);
    printDistance(distance_down);// debug
    Serial.print("(sensor down) "); //debug
    if (distance_down > THRESHOLDDISTANCEMIN
        && distance_down < THRESHOLDDISTANCE)
    {
        if(!timeout_presence_in.started())
        {
            timeout_presence_in.start();
            presence = false;
        }
        else if (timeout_presence_in.done())
            presence = true;
        else
            presence = false;
        resetTimer(&timeout_presence_out);
    }
    else
    {
        if(!timeout_presence_out.started())
            timeout_presence_out.start();
        if (timeout_presence_out.done())
        {
          resetTimer(&timeout_presence_out);
          presence = false;
          resetTimer(&timeout_presence_in);
          
        }
        else if (timeout_presence_in.waiting())
        {
            resetTimer(&timeout_presence_in);
            presence = true;
        }
    }
}


void caseIdle(void)
{
    order = STOP;
    control_motor(order);
    screen_pos = origin;
    resetTimers();
    if (presence)
        state = fiting;
        // state = person_detected;
}

// void casePersonDetected(void)
// {
//     if (!presence)
//         state = screen_reset;
//     else if(timeout_presence_in.done())
//     {
//         timeout_presence_in.reset();
//         timeout_presence_in.stop();
//         state = fiting;
//     }
// }

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
    Serial.print("old order: ");
    Serial.print(old_order);
    Serial.print(", order: ");
    Serial.println(order);
  control_motor(order);
}

void resetScreen(){
  if (screen_pos == unknown) {
    if(!timeout_move_up.started()
    && !timeout_move_up.done()){
        order = UP;
        timeout_move_up.start();
    }
    if(timeout_move_up.done()
    && !timeout_move_down.started()){
        timeout_move_down.start();
        order = DOWN;
    }
  }
  else if (screen_pos == above)
      order = DOWN;
  else if (screen_pos == under)
      order = UP;
  control_motor(order);
}

void caseInUse()
{
    order = STOP;
    control_motor(order);
    if (!presence)
    {
        resetTimers();
        state = screen_reset;
    }
}

void printState(void)
{
    if (presence)
        Serial.print(F("presence,\t"));
    else
        Serial.print(F("!presence,\t"));
    Serial.print("state: ");
    switch (state)
    {
        case idle:
            Serial.println(F("idle"));
            break;
        case fiting:
            Serial.println(F("fiting"));
            break;
        case in_use:
            Serial.println(F("in_use"));
        case screen_reset:
            Serial.println(F("screen_reset"));
            break;
        default:
            break;
    }


}

void computeState(void)
{
    updatePresence();
    printState();
    switch (state)
    {
        case idle:
            caseIdle();
            break;
        case fiting:
            caseFiting();
            break;
        case in_use:
            caseInUse();
        case screen_reset:
            resetScreen();
            break;
        default:
            break;
    }
}

void initTimer(void)
{
    timeout_presence_out.start();
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

    initTimer();

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
  Serial.print(" ISR : ");
  printState();
}

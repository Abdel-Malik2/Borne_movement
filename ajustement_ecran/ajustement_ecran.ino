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

    distance_up = getDistance(&sensor_up);
    Serial.print("GET ADJUST: sensor up distance: ");//debug
    Serial.println(distance_up);//debug
    return (distance_up > THRESHOLDDISTANCEMIN
       && distance_up < THRESHOLDDISTANCE);
}

void printDistance(uint16_t distance_down) //debug
{
    Serial.print(" sensor: ");
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
        if (!timeout_presence_in.started())
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
        if (!timeout_presence_out.started())
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
    resetTimers();
    if (presence)
        state = fiting;
}

void caseFiting(void)
{
    e_motor_control old_order = order;

    order = (getAdjust()) ? UP : DOWN;
    if (old_order != STOP && old_order != order)
    {
        screen_pos = (order) ? above : under;
        order = STOP;
        state = in_use;
    }
  control_motor(order);
}

void print_screen_pos(e_screen_pos screen_pos) //debug
{
    Serial.print(" SCREEN POSITION => ");
    switch (screen_pos)
    {
        case unknown:
        {
            Serial.print(F("unknown"));
            break;
        }
        case origin:
        {
            Serial.print(F("origin"));
            break;
        }
        case above:
        {
            Serial.print(F("above"));
            break;
        }
        case under:
        {
            Serial.print(F("under"));
            break;
        }
        default:
            break;
    }
}

void print_order(e_motor_control order) //debug
{
    Serial.print(" ORDER => ");
    switch (order)
    {
        case UP:
        {
            Serial.print(F("UP\t"));
            break;
        }
        case DOWN:
        {
            Serial.print(F("DOWN\t"));
            break;
        }
        case STOP:
        {
            Serial.print(F("STOP\t"));
            break;
        }
        default:
            break;
    }
}

void resetScreen()
{
    Serial.print("RESET SCREEN: ");

    if (screen_pos == unknown)
    {
        order = UP;
        if(!timeout_move_up.started())
            timeout_move_up.start();
        else if (timeout_move_up.done())
        {
            order = DOWN;
            if(!timeout_move_down.started())
                timeout_move_down.start();        }
    }
    else if (screen_pos == above)
        order = DOWN;
    else if (screen_pos == under)
    {
        order = UP;
    }
    else if (screen_pos == origin)
        state = idle;

    print_order(order); //debug
    print_screen_pos(screen_pos);//debug
    control_motor(order);
}

void caseInUse()
{
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
    Serial.print(F("state: "));
    switch (state)
    {
        case idle:
        {
            Serial.println(F("Idle"));
            break;
        }
        case fiting:
        {
            Serial.println(F("Fiting"));
            break;
        }
        case in_use:
        {
            Serial.println(F("In Use"));
            break;
        }
        case screen_reset:
        {
            Serial.println(F("Screen Reset"));
            break;
        }
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
        {
            caseIdle();
            break;
        }
        case fiting:
        {
            caseFiting();
            break;
        case in_use:
            caseInUse();
            break;
        }
        case screen_reset:
        {
            resetScreen();
            break;
        }
        default:
            break;
    }
}

void initTimer(void)
{
    timeout_presence_out.start();
}

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

/*
 * The method will run when the interrupter is on.
 */
void ISR()
{
    switch(state)
    {
        case screen_reset:
        {
            state = idle;
            resetTimers();
            screen_pos = origin;
            order = STOP;
            break;
        }
        case fiting:
        {
            if(order == UP)
                screen_pos = above;
            if(order == DOWN)
                screen_pos = under;
            break;
        }
        default:
            break;
    }
    Serial.print(" ISR : ");
    printState();
}

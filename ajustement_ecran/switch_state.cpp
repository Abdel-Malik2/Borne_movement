#include "switch_state.h"

void caseIdle(void)
{
    order = STOP;
    control_motor(order);
    resetTimers();
    if (presence)
        state = fiting;
}

void setInUseState(e_screen_pos new_screen_pos = unknown)
{
    if (new_screen_pos == unknown)
        screen_pos = (order) ? above : under;
    else
        screen_pos = new_screen_pos;
    order = STOP;
    state = in_use;
}

void caseFiting(void)
{
    e_motor_control old_order = order;

    order = (getAdjust()) ? UP : DOWN;
    if (old_order != STOP && old_order != order)
        setInUseState();
    if (order == UP)
    {
        if(!timeout_fiting_up.started())
            timeout_fiting_up.start();
        else if (timeout_fiting_up.done())
            setInUseState(above);
    }
    if (order == DOWN)
    {
        if(!timeout_fiting_down.started())
            timeout_fiting_down.start();
        else if (timeout_fiting_down.done())
            setInUseState(under);
    }
    control_motor(order);
}

void resetScreen()
{
    Serial.print(F("RESET SCREEN: "));

    if (screen_pos == unknown)
    {
        order = UP;
        if(!timeout_move_up.started())
            timeout_move_up.start();
        else if (timeout_move_up.done())
        {
            order = DOWN;
            if(!timeout_move_down.started())
                timeout_move_down.start();
        }
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
    printScreenPos(screen_pos);//debug
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
#include "screen_timer.h"

void initTimer(void)
{
    timeout_presence_out.start();
}

void resetTimer(Neotimer *timer)
{
    timer->reset();
    timer->stop();
}

void resetTimers(void)
{
    resetTimer(&timeout_move_up);
    resetTimer(&timeout_move_down);
    resetTimer(&timeout_fiting_up);
    resetTimer(&timeout_fiting_down);
}

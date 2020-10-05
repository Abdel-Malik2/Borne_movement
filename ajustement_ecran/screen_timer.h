#ifndef SCREEN_TIMER_H
# define SCREEN_TIMER_H

# include <Arduino.h>
# include <neotimer.h>

extern Neotimer timeout_move_up;
extern Neotimer timeout_move_down;
extern Neotimer timeout_presence_out;
extern Neotimer timeout_presence_in;
extern Neotimer timeout_fiting_up;
extern Neotimer timeout_fiting_down;

void resetTimers(void);
void resetTimer(Neotimer *timer);
void initTimer(void);

#endif
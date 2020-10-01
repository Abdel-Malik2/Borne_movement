#ifndef ENUMS_H
#define ENUMS_H

/**
 * The given state of the device among all possible
 */
enum e_state {
  idle,
//   person_detected,
  fiting,
  in_use,
  screen_reset
};

/**
 * The relative position of the screen with an origin equal to the interrupter physical position
 */
enum e_screen_pos {
  unknown,
  origin,
  above,
  under
};

enum e_motor_control {
  UP,
  DOWN,
  STOP
};

#endif

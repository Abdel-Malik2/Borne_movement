#ifndef PIN_DEFINE
# define PIN_DEFINE

//Pin de contrôle d'extinction - pour changement d'adresse
# define PIN_XSHUT_1 PB0  //D3 - capteur haut
# define PIN_XSHUT_2 PA12   //D2 - capteur bas

//Futures adresses des capteurs [initialement 0x29]
# define ADR_SENSOR_1 0x24
# define ADR_SENSOR_2 0x42

//Pins pour connexion I2C
# define SCL_Wire PA7      //A6
# define SDA_Wire PB4      //D12

# define interrupter PB7   //D4

//Motor control chip information
# define InA PB6           //D5
# define InB PB1           //D6
# define SEL PA11          //D10
# define PWM PA8           //D9

/**
 * The given state of the device among all possible
 */
enum e_state {
  idle,
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

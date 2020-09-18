#include "hydraulic_cylinder_control.h"

void control_motor(e_motor_control choice, int pwm){
  bool ina,inb,sel;
  switch(choice){
    case UP:
      ina = HIGH;
      inb = LOW;
      sel = LOW;
      break;
    case DOWN:
      ina = LOW;
      inb = HIGH;
      sel = HIGH;
      break;
    case STOP:
      ina = LOW;
      inb = LOW;
      sel = HIGH;
  }
  analogWrite(PWM,pwm);
  digitalWrite(InA,ina);
  digitalWrite(InB,inb);
  digitalWrite(SEL,sel);
}

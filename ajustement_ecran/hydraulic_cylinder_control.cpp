#include "hydraulic_cylinder_control.h"

void initialise_pinmode(){
  pinMode(InA, OUTPUT);
  pinMode(InB, OUTPUT);
  pinMode(SEL ,OUTPUT);
  pinMode(PWM ,OUTPUT);
}
void control_motor(e_motor_control choice, int pwm){
  bool ina,inb,sel;
  switch(choice){
    case UP:
      Serial.println(F("motor:|move_up|"));
      ina = HIGH;
      inb = LOW;
      sel = LOW;
      break;
    case DOWN:
      Serial.println(F("motor:|move_down|"));
      ina = LOW;
      inb = HIGH;
      sel = HIGH;
      break;
    case STOP:
    Serial.println(F("motor:|stop|"));
      ina = LOW;
      inb = LOW;
      sel = HIGH;
  }
  analogWrite(PWM,pwm);
  digitalWrite(InA,ina);
  digitalWrite(InB,inb);
  digitalWrite(SEL,sel);
}

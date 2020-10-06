#include "hydraulic_cylinder_control.h"

void controlMotor(e_motor_control choice, int pwm){
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
  Serial.print("p=");
  Serial.print(pwm);
  Serial.print(", InA=");
  Serial.print(ina);
  Serial.print(", InB=");
  Serial.print(inb);
  Serial.print(", sel=");
  Serial.println(sel);
}

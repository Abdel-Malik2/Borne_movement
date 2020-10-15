#include "hydraulic_cylinder_control.h"

void controlMotor(e_motor_control choice, int pwm){
  bool ina,inb,sel;
  Serial.print(F("motor: "));
  switch(choice){
    case UP:
      Serial.print(F("UP"));
      ina = HIGH;
      inb = LOW;
      sel = LOW;
      if (pwm == 255)       // TEST
        pwm = 200;          // limit PWM
      break;
    case DOWN:
      Serial.print(F("DOWN"));
      ina = LOW;
      inb = HIGH;
      sel = HIGH;
      if (pwm == 255)      // TEST
        pwm = 180;         // limit PWM
      break;
    case STOP:
    Serial.print(F("STOP"));
      ina = LOW;
      inb = LOW;
      sel = HIGH;
      break;
  }
  analogWrite(PWM,pwm);
  digitalWrite(InA,ina);
  digitalWrite(InB,inb);
  digitalWrite(SEL,sel);
  Serial.print(F(" pwm = "));
  Serial.print(pwm);
  Serial.print(F(" | InA = "));
  Serial.print(ina);
  Serial.print(F(" | InB = "));
  Serial.print(inb);
  Serial.print(F(" | sel = "));
  Serial.println(sel);
}

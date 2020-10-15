#include "hydraulic_cylinder_control.h"

void initialise_pinmode(){
  pinMode(InA, OUTPUT);
  pinMode(InB, OUTPUT);
  pinMode(SEL ,OUTPUT);
  pinMode(PWM ,OUTPUT);
}
void controlMotor(e_motor_control choice, int pwm){
  bool ina,inb,sel;
      Serial.print(F("motor: "));

  switch(choice){
    case UP:
      Serial.println(F("Up"));
      ina = HIGH;
      inb = LOW;
      sel = LOW;
      break;
    case DOWN:
      Serial.println(F("Down"));
      ina = LOW;
      inb = HIGH;
      sel = HIGH;
      break;
    case STOP:
      Serial.println(F("Stop"));
      ina = LOW;
      inb = LOW;
      sel = HIGH;
      break;
  }
  analogWrite(PWM,pwm);
  digitalWrite(InA,ina);
  digitalWrite(InB,inb);
  digitalWrite(SEL,sel);
  delay(100);
}

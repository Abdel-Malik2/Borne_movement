#include "pin_define_types.h"
#include "enums.h"
#include "hydraulic_cylinder_control.h"

#define interrupter PB7   //D4

e_motor_control order = STOP;

void setup() {
  Serial.begin(9600);
  Serial.println(F("demarrage"));
  attachInterrupt(digitalPinToInterrupt(interrupter), ISR, RISING);

  pinMode(InA, OUTPUT);
  pinMode(InB, OUTPUT);
  pinMode(SEL ,OUTPUT);
  pinMode(PWM ,OUTPUT);

}

void loop() {
  control_motor(DOWN);
  delay(500);
}

void ISR()
{
    Serial.println("CALL ISR");
}

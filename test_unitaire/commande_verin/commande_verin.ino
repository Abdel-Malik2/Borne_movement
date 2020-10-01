#include "pin_define.h"
#include "enums.h"
#include "hydraulic_cylinder_control.h"

e_motor_control order = STOP;

void setup() {
  Serial.begin(9600);
  Serial.println(F("demarrage"));
  pinMode(InA, OUTPUT);
  pinMode(InB, OUTPUT);
  pinMode(SEL ,OUTPUT);
  pinMode(PWM ,OUTPUT);

}

void loop() {
  control_motor(DOWN);
  delay(500);
}

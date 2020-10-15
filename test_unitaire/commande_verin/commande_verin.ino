#include "hydraulic_cylinder_control.h"

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
    controlMotor(UP,100);
    delay(1500);
    controlMotor(STOP);
    delay(3500);
    controlMotor(DOWN, 50);
    delay(3000);
    controlMotor(STOP);
    delay(3000);
}

void ISR()
{
    Serial.println("CALL ISR");
}

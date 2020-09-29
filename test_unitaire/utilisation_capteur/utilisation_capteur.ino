#include <Wire.h>
#include <VL53L0X.h>

VL53L0X sensor = VL53L0X();

//Pins pour connexion I2C
#define SCL_Wire PA7      //A6
#define SDA_Wire PB4      //D12

void setup()
{
  Serial.begin(9600);
  Wire.setSCL(SCL_Wire);
  Wire.setSDA(SDA_Wire);
  Wire.begin();
  delay(200);
  while(!sensor.init()){
  if (!sensor.init())
  {
    Serial.println("Failed to detect and initialize sensor!");
  }
  }
  delay(200);
  sensor.startContinuous(100);
}
void loop()
{
  Serial.print(sensor.readRangeContinuousMillimeters());
  if (sensor.timeoutOccurred()) { Serial.print(" TIMEOUT"); }
  Serial.println();
}

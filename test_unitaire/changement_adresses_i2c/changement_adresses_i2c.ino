#include <Wire.h> //Native librairy
#include <VL53L0X.h>

//Pin de contrôle d'extinction - pour changement d'adresse
#define PIN_XSHUT_1 PB0  //D3 - capteur haut
#define PIN_XSHUT_2 PA12   //D2 - capteur bas

//Futures adresses des capteurs [initialement 0x29]
#define ADR_SENSOR_1 0x24
#define ADR_SENSOR_2 0x42

//Pins pour connexion I2C
#define SCL_Wire PA7      //A6
#define SDA_Wire PB4      //D12

VL53L0X sensor_up = VL53L0X();
VL53L0X sensor_down = VL53L0X();

void turn_off_sensors(){
  //sensor_up
  pinMode(PIN_XSHUT_1, OUTPUT);
  digitalWrite(PIN_XSHUT_1, LOW);
  //sensor_down
  pinMode(PIN_XSHUT_2, OUTPUT);
  digitalWrite(PIN_XSHUT_2, LOW);
}

void prepareSensor(byte pin, bool stat)
{
  pinMode(pin, OUTPUT);
  digitalWrite(pin, stat);
}

void initSensor(VL53L0X *sensor, byte pin, uint8_t adr)
{
  pinMode(pin, INPUT);
  delay(200);
  Serial.println("Chargement");
  while(!sensor->init()){
    Serial.println("Initialisation Failed");
    delay(100);
  }
  Serial.println("Chargementfini");
  delay(100);
  sensor->setAddress(adr);
  sensor->setTimeout(500);
  sensor->startContinuous(50);
}

void setup() {
  Serial.begin(9600);
  Serial.print("Start\n");
  Wire.setSCL(SCL_Wire);
  Wire.setSDA(SDA_Wire);
  Wire.begin();

  turn_off_sensors();
  delay(200);
  prepareSensor(PIN_XSHUT_1, HIGH);
  initSensor(&sensor_up, PIN_XSHUT_1, ADR_SENSOR_1);
  delay(50);
  prepareSensor(PIN_XSHUT_2, HIGH);
  initSensor(&sensor_down, PIN_XSHUT_2, ADR_SENSOR_2);
}


void loop() {
  int nDevices = 0;

  Serial.println("Scanning...");

  for (byte address = 1; address < 127; ++address) {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    byte error = Wire.endTransmission();

    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address < 16) {
        Serial.print("0");
      }
      Serial.print(address, HEX);
      Serial.println("  !");

      ++nDevices;
    } else if (error == 4) {
      Serial.print("Unknown error at address 0x");
      if (address < 16) {
        Serial.print("0");
      }
      Serial.println(address, HEX);
    }
  }
  if (nDevices == 0) {
    Serial.println("No I2C devices found\n");
  } else {
    Serial.println("done\n");
  }
  delay(2000); // Wait 5 seconds for next scan
}

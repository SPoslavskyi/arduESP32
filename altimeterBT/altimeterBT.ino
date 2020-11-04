#include "BluetoothSerial.h"


#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>


#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

Adafruit_BME280 bme; // I2C

float SEALEVELPRESSURE_HPA;
float curAlt;

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32test"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");

  Serial.println(F("BME280 test"));

  bool status;

  status = bme.begin(0x76);  
  if (!status) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }

  float checkpointAlitude = 140.0;
  Serial.print("Checkpoint altitude: ");
  Serial.println(checkpointAlitude);
  float curPreasure = bme.readPressure() / 100.0F;
  Serial.print("Current preasure: ");
  Serial.println(curPreasure);
  SEALEVELPRESSURE_HPA = bme.seaLevelForAltitude(checkpointAlitude, curPreasure);
  Serial.print("Sea level preasure: ");
  Serial.println(SEALEVELPRESSURE_HPA);

  Serial.println();
  
}

void loop() {
  curAlt = bme.readAltitude(SEALEVELPRESSURE_HPA);
  Serial.println(curAlt);
  SerialBT.println(curAlt);
  if (Serial.available()) {
    SerialBT.write(Serial.read());
  }
  if (SerialBT.available()) {
    Serial.write(SerialBT.read());
  }
  delay(2000);
}

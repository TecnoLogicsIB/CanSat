// inserció de llibreries
#include <Wire.h>
#include <Adafruit_BME280.h>

// creació d'objectes i variables:
Adafruit_BME280 bme; 
float temperatura, pressio, altitud, humitat;
unsigned long temps_referencia = 0;

void setup() 
{
  Serial.begin(115200);
  bme.begin(0x76);
}

void loop() 
{
  if((millis()-temps_referencia)>=1000)
  {
    temps_referencia=millis();
    llegeix_sensors();
    mostra_dades();
  }
}

// ...............................

void llegeix_sensors()
{
  temperatura=bme.readTemperature();
  pressio=bme.readPressure()/100;
  altitud=bme.readAltitude(1013.25);
  humitat=bme.readHumidity();
}

void mostra_dades()
{
  Serial.println (temperatura);
  Serial.println (pressio);
  Serial.println (altitud);
  Serial.println (humitat);
  Serial.println ();
}

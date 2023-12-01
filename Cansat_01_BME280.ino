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
  Serial.print ("temperatura = ");
  Serial.print (temperatura);
  Serial.println (" °C");
  Serial.print ("pressio = ");
  Serial.print (pressio);
  Serial.println (" hPa");
  Serial.print ("altitud aprox = ");
  Serial.print (altitud);
  Serial.println (" m");
  Serial.print ("humitat relativa = ");
  Serial.print (humitat);
  Serial.println (" %");
  Serial.println ();
}

void mostra_dades_1()
{
  Serial.println ("temperatura = " + String(temperatura,2) + " oC");
  Serial.println ("pressio = " + String(pressio,4) + " hPa");
  Serial.println ("altitud aprox = " + String(altitud,2) + " m");
  Serial.println ("humitat relativa = " + String(humitat,2) + " %");
  Serial.println ();
}

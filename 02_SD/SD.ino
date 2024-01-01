// inserció de llibreries
#include <Wire.h>             // per trebalar amb I2C
#include <Adafruit_BME280.h>  // pel sensor BME280
#include "FS.h"               // per gestionar arxius
#include "SD.h"               // per treballar amb targeta SD
#include "SPI.h"              // per treballar amb SPI

// creació d'objectes i variables:
Adafruit_BME280 bme; 
float temperatura, pressio, altitud, humitat;
unsigned long temps_referencia = 0;
int comptador = 0;
const char* arxiu = "/dades.csv";    // nom arxiu a la SD
const char* cabecera = "comptador,altitud_bme (m),temperatura (oC),pressio (hPa),humitat relativa (%), ID \n";   // cabecera arxiu csv
String missatge;  


void setup() 
{
  Serial.begin(115200);
  bme.begin(0x76);
  SD.begin(5);
  deleteFile(SD, arxiu);  // esborra el fitxer si ja existeix
  writeFile(SD, arxiu, cabecera);  //capçalera, sobreescriu el text anterior
}

void loop() 
{
  if((millis()-temps_referencia)>=1000)
  {
    comptador++;
    temps_referencia=millis();
    llegeix_sensors();
    //mostra_dades();
    crea_missatge();
    graba_sd();
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

void crea_missatge()
{
  // cabecera = "comptador,altitud_bme (m),temperatura (oC),pressio (hPa),humitat relativa (%), ID \n";
  missatge = String(comptador)+ "," + String(altitud,2)+ "," + String(temperatura,2)+ "," + String(pressio,2)+ "," + String(humitat,2)+ "," + "BELL-AIR \n";
  //Serial.print missatge;
}

void graba_sd()
{    
  appendFile(SD, arxiu, missatge.c_str());
  readFile(SD, arxiu);
}

// -----------------------------------------------------------------------------

void writeFile(fs::FS &fs, const char * path, const char * message)
{
  //Serial.printf("Writing file: %s\n", path);
  File file = fs.open(path, FILE_WRITE);
  file.print(message);
  file.close();
}

void appendFile(fs::FS &fs, const char * path, const char * message)
{
  //Serial.printf("Appending to file: %s\n", path);
  File file = fs.open(path, FILE_APPEND);
  file.print(message);
  file.close();
}

void deleteFile(fs::FS &fs, const char * path)
{
  Serial.printf("Deleting file: %s\n", path);
  fs.remove(path);
}

void readFile(fs::FS &fs, const char * path)
{
  Serial.printf("Reading file: %s\n", path);
  File file = fs.open(path);
  if(!file)
  {
    Serial.println("Failed to open file for reading");
    return;
  }
}

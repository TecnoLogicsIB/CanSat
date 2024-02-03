// inserció de llibreries
#include <Wire.h>             // per trebalar amb I2C
#include <Adafruit_BME280.h>  // pel sensor BME280
#include "FS.h"               // per gestionar arxius
#include "SD.h"               // per treballar amb targeta SD
#include "SPI.h"              // per treballar amb SPI
#include <HardwareSerial.h>   // definició pins Serie
#include <TinyGPSPlus.h>      // pel GPS

// creació d'objectes i variables:
Adafruit_BME280 bme; 
TinyGPSPlus gps;  // anomenem gp a l'objecte TinyGPSPlus
HardwareSerial SerialRF(2);   // nou port sèrie (UART2)
HardwareSerial gps_serial(1); // utilitzo UART1 i l'anomeno gps_serial

String temperatura, pressio, altitud, humitat;     // variables per desar les lectures del BME280 (en format text)
String latitud; String longitud; String altitud_gps; String sats; // variables pèr desar les lectures del gps (en format text)

unsigned long temps_referencia = 0;
int comptador = 0;                   // comptador de paquets
const char* arxiu = "/dades.csv";    // nom arxiu a la SD
const char* cabecera = "comptador,latitud,longitud,altitud GPS (m),altitud BME280 (m),temperatura (oC),pressio (hPa),humitat relativa (%),satelits,ID \n";  // cabecera arxiu csv
String missatge;  

void setup() 
{
  Serial.begin(112500);
  SerialRF.begin(9600, SERIAL_8N1, 16, 17);   // velocitat del mòdul RF, mode de funcionament, pin RX, pin TX
  gps_serial.begin(9600, SERIAL_8N1, 2, 15);  // velocitat del mòdul gps, mode de funcionament, pin RX, pin TX
  bme.begin(0x76);
  SD.begin(5);
  deleteFile(SD, arxiu);  // esborra el fitxer si ja existeix
  writeFile(SD, arxiu, cabecera);  //capçalera, sobreescriu el text anterior
}

void loop() 
{
  while (gps_serial.available() > 0)
  {
    if (gps.encode(gps_serial.read()))
    { llegeix_gps(); }
  }

  if((millis()-temps_referencia)>=1000)
  {
    temps_referencia=millis();
    comptador++;
    llegeix_sensors();
    crea_missatge();
    graba_sd();
    envia_RF();
  }
}

// ...............................

void llegeix_sensors()
{
  temperatura = String(bme.readTemperature(),2);  // en format text, amb 2 decimals
  pressio = String(bme.readPressure()/100,2);
  altitud = String(bme.readAltitude(1013.25),2);
  humitat = String(bme.readHumidity(),2);
}

void llegeix_gps() 
{
	latitud = String(gps.location.lat(), 6);
	longitud = String(gps.location.lng(), 6);
	altitud_gps = String(gps.altitude.meters(),2);
  sats = String(gps.satellites.value());
}

void crea_missatge()
{
  // per comprovar: cabecera = "comptador,latitud,longitud,altitud GPS (m),altitud BME280 (m),temperatura (oC),pressio (hPa),humitat relativa (%),satelits,ID \n" 
  missatge = String(comptador)+","+latitud+","+longitud+","+altitud_gps+","+altitud+","+temperatura+","+pressio+","+humitat+","+sats+",BELL-AIR \n";  
}

void graba_sd()
{    
  appendFile(SD, arxiu, missatge.c_str());
  //readFile(SD, arxiu);
}

void envia_RF()
{
  SerialRF.println(missatge.c_str());  // missatge a enviar per RF
  Serial.println (missatge);  // per comprovació
}
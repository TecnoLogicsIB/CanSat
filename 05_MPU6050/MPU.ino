// inserció de llibreries
#include <Wire.h>              // per trebalar amb I2C
#include <Adafruit_BME280.h>   // pel sensor BME280
#include "FS.h"                // per gestionar arxius
#include "SD.h"                // per treballar amb targeta SD
#include "SPI.h"               // per treballar amb SPI
#include <HardwareSerial.h>    // definició pins Serie
#include <TinyGPSPlus.h>       // pel GPS
#include <Adafruit_MPU6050.h>  // pel MPU6050
#include <Adafruit_Sensor.h>   // pel MPU6050

// creació d'objectes i variables:
Adafruit_BME280 bme; 
TinyGPSPlus gps;       // anomenem gp a l'objecte TinyGPSPlus
Adafruit_MPU6050 mpu;  // objecte MPU6050
HardwareSerial SerialRF(2);   // nou port sèrie (UART2)
HardwareSerial gps_serial(1); // utilitzo UART1 i l'anomeno gps_serial

String temperatura, pressio, altitud, humitat;     // variables per desar les lectures del BME280 (format núm decimal)
String latitud; String longitud; String altitud_gps; String sats; // variables pèr desar les lectures del gps (format text)
float ax, ay, az;  // valors d'acceleració
String accel;

unsigned long temps_referencia = 0;
int comptador = 0;
const char* arxiu = "/dades.csv";    // nom arxiu a la SD
const char* cabecera = "comptador,latitud,longitud,altitud GPS (m),altitud BME280 (m),temperatura (oC),pressio (hPa),humitat relativa (%),satelits,acceleració (m/s2),ID \n";  // cabecera arxiu csv
String missatge;  

void setup() 
{
  Serial.begin(112500);
  SerialRF.begin(9600, SERIAL_8N1, 16, 17);  // velocitat del mòdul RF, mode de funcionament, pin RX, pin TX
  gps_serial.begin(9600, SERIAL_8N1, 2, 15);  // velocitat del mòdul gps, mode de funcionament, pin RX, pin TX
  bme.begin(0x76);
  SD.begin(5);
  deleteFile(SD, arxiu);  // esborra el fitxer si ja existeix
  writeFile(SD, arxiu, cabecera);  //capçalera, sobreescriu el text anterior
  mpu.begin(0x68);   // inicialització mpu
  mpu.setAccelerometerRange(MPU6050_RANGE_16_G);   // estableix el rang de mesura de l'acceleròmetre
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
    comptador++;
    temps_referencia=millis();
    llegeix_sensors();
    crea_missatge();
    graba_sd();
    envia_RF();
  }
}

// ...............................

void llegeix_sensors()
{
  temperatura= String(bme.readTemperature(),2);
  pressio=String(bme.readPressure()/100,2);
  altitud=String(bme.readAltitude(1013.25),2);
  humitat=String(bme.readHumidity(),2);
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  ax= a.acceleration.x; ay= a.acceleration.y; az= a.acceleration.z;
  accel = String(sqrt(pow(ax,2) + pow(ay,2) + pow(az,2)),2);  // vector aceleración: la raiz de la suma de los componentes al cuadrado (pow,2)
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
  //missatge = String(comptador)+ "," + latitud + "," + longitud + "," + altitud_gps + "," + String(altitud,2) + "," +String(temperatura,2)+ "," +String(pressio,2)+ "," +String(humitat,2)+ "," + "BELL-AIR \n"; 
  missatge = String(comptador)+","+latitud+","+longitud+","+altitud_gps+","+altitud+","+temperatura+","+pressio+","+humitat+","+sats+","+accel+",BELL-AIR \n";  
}

void graba_sd()
{    
  appendFile(SD, arxiu, missatge.c_str());
  //readFile(SD, arxiu);
}

void envia_RF()
{
  SerialRF.println(missatge.c_str());  // missatge a enviar per RF
  //Serial.println (missatge);  // per comprovació
}

// ----------------------- FUNCIONS SD ----------------------

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

void readFile(fs::FS &fs, const char * path){
    Serial.printf("Reading file: %s\n", path);

    File file = fs.open(path);
    if(!file){
        Serial.println("Failed to open file for reading");
        return;
    }

    Serial.print("Read from file: ");
    while(file.available()){
        Serial.write(file.read());
    }
    file.close();
}

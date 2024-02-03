#include <TinyGPSPlus.h>     // per treballar amb NEO-6M
#include <HardwareSerial.h>  // per definir el port sèrie

TinyGPSPlus gps;  // anomenem gp a l'objecte TinyGPSPlus
HardwareSerial gps_serial(1); // utilitzo UART1 i l'anomeno gps_serial

int comptador=0;

// variables pèr desar les lectures del gps (format text)
String latitud;
String longitud;
String altitud;
String sats;

unsigned long temps_referencia = 0;

void setup() 
{
  Serial.begin(115200);
  gps_serial.begin(9600, SERIAL_8N1, 2, 15);  // velocitat del mòdul gps, mode de funcionament, pin RX, pin TX
}

void loop() 
{
  while (gps_serial.available() > 0)
  {
    if (gps.encode(gps_serial.read()))
    { llegir_gps(); }
  }

  if((millis()-temps_referencia)>=1000)
  { 
    temps_referencia=millis();
    comptador++;
    enviar_serie(); 
  }
}

void llegir_gps() 
{
	latitud = String(gps.location.lat(), 6);
	longitud = String(gps.location.lng(), 6);
	altitud = String(gps.altitude.meters(),2);
  sats = String(gps.satellites.value());  // nombre de satèlits detectat (+ sats -> + precissió)
}

void enviar_serie() 
{
  Serial.print(comptador);
  Serial.print(",");
	Serial.print(latitud);
	Serial.print(",");
	Serial.print(longitud);
	Serial.print(",");
	Serial.print(altitud);
  Serial.print(",");
	Serial.println(sats);
}

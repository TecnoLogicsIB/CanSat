#include "FS.h"   // per gestionar arxius
#include "SD.h"   // per treballar amb SD
#include "SPI.h"  // per traballar amb SDI

void setup() 
{
  SD.begin(5);   // inicialització de la SD indicant el pin CS
  //writeFile(SD, "/hello.txt", "Hello \n");
  writeFile(SD, "/dades.csv", "Hello \n");  
}

void loop() 
{
  //appendFile(SD, "/hello.txt", "World!\n");
  appendFile(SD, "/dades.csv", "World!\n");  
  delay(1000);
}

// ........................

void writeFile(fs::FS &fs, const char * path, const char * message)  // escriu contingut en un arxiu. Si l'arxiu no existeix el crearà
{
  //Serial.printf("Writing file: %s\n", path);
  File file = fs.open(path, FILE_WRITE);
  file.print(message);
  file.close();
}

void appendFile(fs::FS &fs, const char * path, const char * message)  // afegeix contingut a l'arxiu especificat
{
  //Serial.printf("Appending to file: %s\n", path);
  File file = fs.open(path, FILE_APPEND);
  file.print(message);
  file.close();
}

void deleteFile(fs::FS &fs, const char * path)  // elimina l'arxiu especificat
{
  //Serial.printf("Deleting file: %s\n", path);
  fs.remove(path);
}

void readFile(fs::FS &fs, const char * path)  // mostra el contingut de l'arxiu epl monitor sèrie
{
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

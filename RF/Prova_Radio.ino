#include <HardwareSerial.h>  // definició pins Serie

HardwareSerial SerialRF(2); // nou port sèrie (UART2)

void setup()
{
  Serial.begin(115200); // velocitat de comunicació estandar
  SerialRF.begin(9600, SERIAL_8N1, 16, 17);  // velocitat del mòdul RF, mode de funcionament, pin RX, pin TX
}

void loop()
{
  Serial.println("enviant");   // missatge enviat per l'emissor
  SerialRF.println("rebent");  // missatge rebut pel receptor
  delay(1000);
}

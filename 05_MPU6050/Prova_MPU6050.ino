#include <Adafruit_MPU6050.h>  // per treballar amb MPU6050
#include <Wire.h>              // per treballar amb I2C

Adafruit_MPU6050 mpu;          // objecte MPU6050

float ax, ay, az, accel;  // valors d'acceleració
float gx, gy, gz;         // valors de gir
float temp_6050;          // valor de temperatura. La temperatura que mide es interna, en el interior del chip.

void setup(void) 
{
  Serial.begin(115200);
  mpu.begin(0x68);  // inicialitza el sensor

  mpu.setAccelerometerRange(MPU6050_RANGE_16_G);     // estableix el rang de mesura de l'acceleròmetre
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);         // estableix el rang de mesura del giroscopi
  //mpu.setFilterBandwidth(MPU6050_BAND_260_HZ);     // estableix l’amplada de banda del filtre (?)
}

void loop() 
{
  /* Get new sensor events with the readings */
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  // obtenció de lesctures del sensor:
  ax= a.acceleration.x;
  ay= a.acceleration.y;
  az= a.acceleration.z;
  accel = sqrt(pow(ax,2) + pow(ay,2) + pow(az,2));  // vector aceleración: la raiz cuadrada (sqrt) de la suma de los componentes al cuadrado (pow(),2)

  gx= g.gyro.x;
  gy= g.gyro.y;
  gz= g.gyro.z;

  temp_6050= temp.temperature;

  //imprime();
  grafica();
  delay(500);
}

void imprime()  // mostra al monitor serie
{
  Serial.print("Acceleration X: ");
  Serial.print(ax);
  Serial.print(", Y: ");
  Serial.print(ay);
  Serial.print(", Z: ");
  Serial.print(az);
  Serial.print(", acceleracio total: ");
  Serial.print(accel);
  Serial.println(" m/s^2");

  Serial.print("Rotation X: ");
  Serial.print(gx);
  Serial.print(", Y: ");
  Serial.print(gy);
  Serial.print(", Z: ");
  Serial.print(gz);
  Serial.println(" rad/s");

  Serial.print("Temperature: ");
  Serial.print(temp_6050);
  Serial.println(" degC");

  Serial.println("");
}

void grafica()  // per veure en serial plotter (valors separats per comes)
{
  //Serial.print(ax);
  //Serial.print(",");
  //Serial.print(ay);
  //Serial.print(",");
  Serial.print(az);
  Serial.print(",");
  Serial.println(accel);
  //Serial.print(gx);
  //Serial.print(",");
  //Serial.print(gy);
  //Serial.print(",");
  //Serial.println(gz);
}

#include <Arduino.h>
#include <CO2Sensor.h>
#include <SoftwareSerial.h>
 
//Variaveis do sensor de CO2
int PinCO = A0;

CO2Sensor sensor(PinCO, 0.99, 100);

//intervalo entre registos
#define INTERVAL 1000 

#define LENG 31   //0x42 + 31 bytes equal to 32 bytes
unsigned char buf[LENG];

int PM01Value=0;          //define PM1.0 value of the air detector module
int PM2_5Value=0;         //define PM2.5 value of the air detector module
int PM10Value=0;         //define PM10 value of the air detector module

SoftwareSerial PMSerial(2, 3); // RX, TX
//SoftwareSerial PMSerial(3, 2); // RX, TX

void setup()
{
  PMSerial.begin(9600);
  sensor.calibrate();
  PMSerial.setTimeout(1500);
  Serial.begin(115200);

  delay(INTERVAL*5);
}

void loop()
{
  if(PMSerial.find(0x42)){
    delay(100);
    PMSerial.readBytes(buf,LENG);

    if(buf[0] == 0x4d){
      if(checkValue(buf,LENG)){
        PM01Value=transmitPM01(buf); //count PM1.0 value of the air detector module
        PM2_5Value=transmitPM2_5(buf);//count PM2.5 value of the air detector module
        PM10Value=transmitPM10(buf); //count PM10 value of the air detector module
      }
    }
  }

  static unsigned long OledTimer=millis();
    if (millis() - OledTimer >=INTERVAL)
    {
      OledTimer=millis();

      int value = sensor.read();

      //Print no formato "%CO2;PM1.0;PM2.5;PM10$"
      Serial.print("%");
      Serial.print(value);
      Serial.print(";");

      Serial.print(PM01Value);
      Serial.print(";");

      Serial.print(PM2_5Value);
      Serial.print(";");

      Serial.print(PM10Value);
      Serial.print(";");

      Serial.print("$");
      Serial.println();
    }

}

char checkValue(unsigned char *thebuf, char leng)
{
  char receiveflag=0;
  int receiveSum=0;

  for(int i=0; i<(leng-2); i++){
  receiveSum=receiveSum+thebuf[i];
  }
  receiveSum=receiveSum + 0x42;

  if(receiveSum == ((thebuf[leng-2]<<8)+thebuf[leng-1]))  //check the serial data
  {
    receiveSum = 0;
    receiveflag = 1;
  }
  return receiveflag;
}

int transmitPM01(unsigned char *thebuf)
{
  int PM01Val;
  PM01Val=((thebuf[3]<<8) + thebuf[4]); //count PM1.0 value of the air detector module
  return PM01Val;
}

//transmit PM Value to PC
int transmitPM2_5(unsigned char *thebuf)
{
  int PM2_5Val;
  PM2_5Val=((thebuf[5]<<8) + thebuf[6]);//count PM2.5 value of the air detector module
  return PM2_5Val;
}

//transmit PM Value to PC
int transmitPM10(unsigned char *thebuf)
{
  int PM10Val;
  PM10Val=((thebuf[7]<<8) + thebuf[8]); //count PM10 value of the air detector module
  return PM10Val;
}
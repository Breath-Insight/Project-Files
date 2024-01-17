#include <Adafruit_SPIDevice.h>
#include <Adafruit_BusIO_Register.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_I2CRegister.h>

#include <Adafruit_PM25AQI.h>

unsigned long intervalo = 1000; //intervalo de tempo entre amostras

//Variaveis do sensor de PM2.5
Adafruit_PM25AQI pms = Adafruit_PM25AQI();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  if(!pms.begin_UART(&pmsSerial)){
    Serial.println("Não foi encontrado um sensor de PM2.5");
    while(1)delay(10);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  static unsigned long timer;
  if(millis()-timer>=intervalo){
    PM25_AQI_Data data;
	
    //Print de PM2.5
    Serial.print(data.pm25_standard);
  }
}

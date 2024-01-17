#include <SoftwareSerial.h>

unsigned long intervalo = 1000; //intervalo de tempo entre amostras

//Variaveis do sensor de PMs
SoftwareSerial pmsSerial(2, 3);

//Variaveis Teste
#define LENGTH 31 //Numero de valores(bytes) que o sensor envia - 1
unsigned char buff[LENGTH]; //Buffer que contem os valores enviados pelo sensor

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

//struct que armazena os valores do buffer
struct pms5003data {
  uint16_t framelen;
  uint16_t pm10_standard, pm25_standard, pm100_standard;
  uint16_t pm10_env, pm25_env, pm100_env;
  uint16_t particles_03um, particles_05um, particles_10um, particles_25um, particles_50um, particles_100um;
  uint16_t unused;
  uint16_t checksum;
};
 
struct pms5003data data;

void loop() {
    static unsigned long timer;
    if(millis() - timer >=intervalo){ //timer para registar as amostras
      if(readPMSdata(&pmsSerial)){ //leitura do sensor
        //Print de PM2.5
        Serial.println(data.pm25_standard);
      }
      timer=millis();
    }
}

boolean readPMSdata(SoftwareSerial *s) {
  if (! s->available()) {
    return false;
    Serial.println("Availability fail"); //debugging
  }
  
  // Read a byte at a time until we get to the special '0x42' start-byte
  if (s->peek() != 0x42) {
    s->read();
    return false;
    Serial.println("0x42 fail"); //debugging
  }
 
  // Now read all 32 bytes
  if (s->available() < 32) {
    return false;
    Serial.println("Byte Check fail"); //debugging
  }
    
  uint8_t buffer[32];    
  uint16_t sum = 0;
  s->readBytes(buffer, 32);
 
  // get checksum ready
  for (uint8_t i=0; i<30; i++) {
    sum += buffer[i];
  }
 
  // debugging
  for (uint8_t i=2; i<32; i++) {
    Serial.print("0x"); Serial.print(buffer[i], HEX); Serial.print(", ");
  }
  Serial.println();
  
  
  // The data comes in endian'd, this solves it so it works on all platforms
  uint16_t buffer_u16[15];
  for (uint8_t i=0; i<15; i++) {
    buffer_u16[i] = buffer[2 + i*2 + 1];
    buffer_u16[i] += (buffer[2 + i*2] << 8);
  }
 
  // put it into a nice struct :)
  memcpy((void *)&data, (void *)buffer_u16, 30);
 
  if (sum != data.checksum) {
    Serial.println("Checksum failure"); //debugging
    return false;
  }
  // success!
  return true;
}

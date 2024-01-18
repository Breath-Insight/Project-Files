#include <Adafruit_PM25AQI.h>
#include <SoftwareSerial.h>
SoftwareSerial pmSerial(2, 3);

// Intervalo de tempo entre amostras
#define INTERVAL 1000

// Criação do objecto do sensor PM2.5
Adafruit_PM25AQI pms = Adafruit_PM25AQI();
// Criação da variável do sensor PM2.5
PM25_AQI_Data data;

void setup() {
	// Configuração para a porta de comunicação série com o PC
	Serial.begin(115200);
	while (!Serial)
		delay(INTERVAL);
	Serial.println("Porta série do PC configurada!");

	// Configuração para a porta de comunicação série com o sensor
	pmsSerial.begin(9600);
	if (!pms.begin_UART(&pmsSerial)) {
		Serial.println("Não foi encontrado um sensor de PM2.5");
		while(1)
			delay(INTERVAL);
	}
	Serial.println("Porta série do sensor configurada!");
}

void loop() {
	// Pausa na execução 
	delay(INTERVAL);

	// Realiza a leitura do sensor PM2.5 e guarda na variável data
	if (!pms.read(&data)) {
		Serial.println("Não foi possível ler dados do sensor");
	} else {
		Serial.print(F("PM 2.5: "));
		Serial.println(data.pm25_standard);
	}
}

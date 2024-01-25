/* 
27/11/2023
Programa de processamento de dados

-Retirar os dados em tempo real do Serial do Arduino
  -Dados em formato "%CO2;PM1.0;PM2.5;PM10$"
-Scan da string do Serial
-Registar hora do registo
-Colocar registo numa tabela
-Exportar como CSV
*/

import processing.serial.*;

//Porta
Serial port;
String buffer;

//Informação
int inicioData = 37; //decimal de % em ASCII
int fimData = 36; //decimal de $ em ASCII
int tempoFinal = 1*60*1000;

//Tabela
Table table;
String path = "C:/Users/user/Documents/Escola/QUIMICA/PROJETO/Registos/registos.csv"; //caminho para o ficheiro de registo cvs

void setup(){  
  //escolher porta para retirar valores
  printArray(Serial.list());
  port = new Serial(this,Serial.list()[0],115200);
  
  print("Serial Connection Success");
  
  //Setup da tabela
  table = new Table();
  table.addColumn("Hora do registo");
  table.addColumn("Milisegundos Decorridos");
  table.addColumn("Concentração de CO2 (ppm)");
  table.addColumn("PM1.0 (µg/m3)");
  table.addColumn("PM2.5 (µg/m3)");
  table.addColumn("PM10.0 (µg/m3)");
}

//Escrever nova linha na tabela dos registos
private void WriteNewLine(Table t, float co2, float pm10, float pm25, float pm100, int ms){
  TableRow row = t.addRow();
  //Receber a hora
  int s = second();
  int m = minute();
  int h = hour();

  String time = String.format("%dh %dmin %dsec", h, m, s);
  row.setString("Hora do registo", time);
  row.setString("Milisegundos Decorridos", Integer.toString(ms));
  row.setFloat("Concentração de CO2 (ppm)", co2);
  row.setFloat("PM1.0 (µg/m3)", pm10);
  row.setFloat("PM2.5 (µg/m3)", pm25);
  row.setFloat("PM10.0 (µg/m3)", pm100);
  print(table.getRowCount());
}

void draw(){
  while(port.available()>0){
    if(port.read()==inicioData){
      int mil = millis();
      String[] token;
      float c, pm100, pm25, pm10;
      buffer = port.readStringUntil(fimData);

      if(buffer!=null){
        //print(buffer);
        //tokenization
        token = splitTokens(buffer, ";");
        if(token.length<2){
          println("Erro de Delimitação");
          return;
        }
        c = parseFloat(token[0]);
        pm10 = parseFloat(token[1]);
        pm25 = parseFloat(token[2]);
        pm100 = parseFloat(token[3]);
        WriteNewLine(table, c, pm10, pm25, pm100, mil);
      }      
      if(mil==tempoFinal){
        print("Fim do registo");
        saveTable(table, path);
        exit();
      }
    }
  }
}

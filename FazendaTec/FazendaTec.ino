/*
  Documentação:
  URL BASE: HTTP://192.168.0.175/?

  Funções:
    -> Ligar bomba URI: rele1=1;
    -> Desligar bomba URI: rele1=0;
    -> Programação automática URI: auto=2;
    -> Ligar RELE2 URI: rele2=1;
    -> Desligar RELE2 URI: rele2=0;
    -> Status URI: status

  Operacionalização:

    O cliente inicia conexão do tipo GET com a solicitação desejada e o Webserver retorna com o status.


  LOW - Pino desligado sem passagem de corrente elétrica.
  HIGH - Pino ligado há passagem de corrente elétrica.
*/
#include <SPI.h>
#include <Ethernet.h>

//DEFINIÇÃO DA CONFIGURAÇÃO DO ARDUINO NA REDE LOCAL.
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; 
byte ip[] = { 192, 168, 0, 175 };
byte gateway[] = {192, 168, 0, 1}; 
byte subnet[] = {255, 255, 255, 0};
EthernetServer server(80); 
//Definição dos sensores de leituras
int rele1 = 6; 
int rele2 = 5; 
int sensorInfer = 7; 
int sensorInter = 8; 
int sensorSuper = 9; 
//Definição de variáveis
String readString = String(30); 
String bomba = "desligada"; 
String programacao = "auto";

void setup() {
  Ethernet.begin(mac, ip, gateway, subnet);  //PASSA OS PARÂMETROS PARA A FUNÇÃO QUE VAI FAZER A CONEXÃO COM A REDE
  server.begin(); //INICIA O SERVIDOR PARA RECEBER DADOS NA PORTA 80
  //Define os pinos como saída
  pinMode(rele1, OUTPUT); 
  pinMode(rele2, OUTPUT);
  pinMode(sensorInfer, OUTPUT);
  pinMode(sensorInter, OUTPUT);
  pinMode(sensorSuper, OUTPUT);
}


void atualizarStatus(){
  if (digitalRead(rele1) == HIGH) {
    bomba = "desligada";
  } if (digitalRead(rele1) == LOW) {
    bomba = "ligada";
  }
}


void loop(){
  EthernetClient client = server.available(); 
  if (client) { 
    while (client.connected()) {
      if (client.available()) { 
        char c = client.read();
        if (readString.length() < 100)
        {
          readString += c; 
        }  
        if (c == '\n') { 
          if (readString.indexOf("?") <0){ 
          }
          else //SENÃO,FAZ
        if(readString.indexOf("rele1=1") >0) 
           {
             digitalWrite(rele1,LOW ); 
             programacao = "manual";
             bomba = "ligada";
           }else{
             digitalWrite(rele1, HIGH); 
             programacao = "manual"; 
             bomba = "ligada";             
           }
          atualizarStatus();           
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: application/json");
          client.println("Connection: close"); 
          client.println();
          client.print("{ 'status': {'bomba': ");
          client.print(bomba);
          client.print(", 'programacao': ");
          client.print(programacao);
          client.print(", 'rele2': ");
          client.print(digitalRead(rele2));
          client.print(", 'sensorInfer': ");
          client.print(digitalRead(sensorInfer));
          client.print(", 'sensorInter': ");
          client.print(digitalRead(sensorInter));
          client.print(", 'sensorSuper': ");
          client.print(digitalRead(sensorSuper));
          client.println("}}");
          readString="";
          client.stop(); 
          break;
       }
      }
     }
   }
 }

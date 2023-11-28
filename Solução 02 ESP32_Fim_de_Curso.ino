// Bibliotecas Usadas
#include <PubSubClient.h>  // Biblioteca para comunicação MQTT
#include <WiFi.h>  // Biblioteca para conexão Wi-Fi
#include "esp_wpa2.h"  // Biblioteca para autenticação WPA2-Enterprise

// Configuração da conexão wireless do ESP32 no modo PEAP sem autenticação
#define EAP_IDENTITY "xxxx"  // Identidade para autenticação PEAP
#define EAP_USERNAME "xxxx"  // Nome de usuário para autenticação PEAP
#define EAP_PASSWORD "xxxx"  // Senha para autenticação PEAP
const char* ssid = "xxxx";  // SSID da rede Wi-Fi

// Inicialização dos objetos das Classes das bibliotecas utilizadas
WiFiClient espClient;  // Objeto para conexão Wi-Fi
PubSubClient client(espClient);  // Objeto para comunicação MQTT

// Configuração das portas utilizadas na ESP32
const int S0 = 2;  // Porta para o sensor fim de curso
const int S1 = 4;  // Porta para o sensor fim de curso

// Configuração do servidor broker
const char* mqttServer = "broker.hivemq.com";  // Endereço do servidor MQTT
const int mqttPort = 1883;  // Porta do servidor MQTT

// Protótipo das funções
void reconnectWiFi();  // Função para reconectar Wi-Fi
void reconnectMQTT();  // Função para reconectar MQTT
void publishSensorState(int sensorValue);  // Função para publicar o estado do sensor

void setup() {
  Serial.begin(115200);  // Inicia a comunicação serial em 115200 bauds
  delay(10);
  Serial.println();
  Serial.print("Conectando ao WIFI: ");
  Serial.println(ssid);
  WiFi.disconnect(true);  // Desconecta do Wi-Fi para configurar nova conexão Wi-Fi
  WiFi.mode(WIFI_STA);  // Inicializa o modo Wi-Fi

  // Conexão à rede Wi-Fi usando o protocolo WPA2-Enterprise com PEAP
  WiFi.begin(ssid, WPA2_AUTH_PEAP, EAP_IDENTITY, EAP_USERNAME, EAP_PASSWORD);


  pinMode(S0, INPUT_PULLUP);  // Define o sensor fim de curso S0 como INPUT_PULLUP
  pinMode(S1, INPUT_PULLUP);  // Define o sensor fim de curso S1 como INPUT_PULLUP

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (WiFi.status() == WL_CONNECT_FAILED || WiFi.status() == WL_CONNECTION_LOST) {
      reconnectWiFi();  // Reconecta Wi-Fi em caso de falha
    }
  }

  Serial.println("");
  Serial.println("ESP32 conectado");
  Serial.println("IP:");
  Serial.println(WiFi.localIP());

  client.setServer(mqttServer, mqttPort);  // Configura o servidor MQTT
  reconnectMQTT();  // Reconecta MQTT
}

void loop(){
 if (!client.connected()) {
    reconnectMQTT();  // Reconecta MQTT se a conexão foi perdida
  }

  client.loop();  // Mantém a comunicação MQTT ativa

  //variavel auxilar para tratar dados do sensores
  int sensorValue = digitalRead(S1);  // Lê o estado do sensor fim de curso S1
  publishSensorState(sensorValue);  // Publica o estado do sensor

  delay(1000); // intervalo de 1 segundo entre as leituras
}

// Função para reconectar Wi-Fi
void reconnectWiFi() {
  Serial.println("WiFi: Falha na conexão. Reconectando a ESP32...");

  int counter = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    counter++;

    if (counter >= 60) {
      ESP.restart();  // Reinicia o ESP32 após 30 segundos de tentativas de conexão
    }
  }

  Serial.println("");
  Serial.println("ESP32 reconectado no WIFI");
  Serial.println("IP address:");
  Serial.println(WiFi.localIP());
}

// Função para reconectar MQTT
void reconnectMQTT() {
  while (!client.connected()) {
    Serial.println("Conectando ao Servidor MQTT broker...");

    if (client.connect("Esp32Sensores")) {
      Serial.println("Conectado ao Servidor MQTT broker");

      const char* msgvalA_status = "ESP32_Sensores Conectado";
      client.publish("Sensores_A/status", msgvalA_status);  // Publica o status do ESP32 

      //client.subscribe("valvA/comando");  // Subscreve no tópico MQTT para receber comandos
    } else {
      Serial.print("Falha na conexão com o Servidor MQTT broker. Reconectando em 5 seconds...");
      delay(5000);
    }
  }
}

// Função para publicar o estado do sensor
void publishSensorState(int sensorValue) {
  if (sensorValue == LOW) {
    //Serial.println("Atuador A Avançado");
    client.publish("Sensores/sensorA0A1", "Atuador A Avançado");
    Serial.println("\n Atuador A Avançado \n");
  } else {
    //Serial.println("Atuador A Recuado");
    client.publish("Sensores/sensorA0A1", "Atuador A Recuado");
    Serial.println("\n Atuador A Recuado \n");
  }
}



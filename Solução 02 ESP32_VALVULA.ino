// Bibliotecas Usadas
#include <PubSubClient.h>  // Biblioteca para comunicação MQTT
#include <WiFi.h>  // Biblioteca para conexão Wi-Fi
#include "esp_wpa2.h"  // Biblioteca para autenticação WPA2-Enterprise
// Configuração da conexão wireless do ESP32 no modo PEAP sem autenticação
#define EAP_IDENTITY "xxxx"  // Identidade para autenticação PEAP
#define EAP_USERNAME "xxxx"  // Nome de usuário para autenticação PEAP
#define EAP_PASSWORD "xxxxx"  // Senha para autenticação PEAP
const char* ssid = "eduroam";  // SSID da rede Wi-Fi
// Inicialização dos objetos das Classes das bibliotecas utilizadas
WiFiClient espClient;  // Objeto para conexão Wi-Fi
PubSubClient client(espClient);  // Objeto para comunicação MQTT
// Configuração das portas utilizadas na ESP32
const int valvA = 5;  // Porta para a válvula A
// Configuração do servidor broker
const char* mqttServer = "broker.hivemq.com";  // Endereço do servidor MQTT
const int mqttPort = 1883;  // Porta do servidor MQTT

// Protótipo das funções
void reconnectWiFi();  // Função para reconectar Wi-Fi
void reconnectMQTT();  // Função para reconectar MQTT
void callback(char* topic, byte* message, unsigned int length);  // Função de retorno de chamada MQTT
void openValve();  // Função para abrir a válvula
void closeValve();  // Função para fechar a válvula
void setup() {
  Serial.begin(115200);  // Inicia a comunicação serial em 115200 bauds
  delay(10);
  Serial.println();
  Serial.print("Connecting to network: ");
  Serial.println(ssid);
  WiFi.disconnect(true);  // Desconecta do Wi-Fi para configurar nova conexão Wi-Fi
  WiFi.mode(WIFI_STA);  // Inicializa o modo Wi-Fi
  // Conexão à rede Wi-Fi usando o protocolo WPA2-Enterprise com PEAP
  WiFi.begin(ssid, WPA2_AUTH_PEAP, EAP_IDENTITY, EAP_USERNAME, EAP_PASSWORD);
  pinMode(valvA, OUTPUT);  // Define a saída para a válvula A como OUTPUT
  digitalWrite(valvA, LOW);  // Inicialmente, a válvula A está fechada (LOW)
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (WiFi.status() == WL_CONNECT_FAILED || WiFi.status() == WL_CONNECTION_LOST) {
      reconnectWiFi();  // Reconecta Wi-Fi em caso de falha
    }
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address:");
  Serial.println(WiFi.localIP());
  client.setServer(mqttServer, mqttPort);  // Configura o servidor MQTT
  client.setCallback(callback);  // Configura a função de retorno de chamada MQTT
  reconnectMQTT();  // Reconecta MQTT
}
void loop() {
  if (!client.connected()) {
    reconnectMQTT();  // Reconecta MQTT se a conexão foi perdida
  }
  client.loop();  // Mantém a comunicação MQTT ativa
  delay(10);
}
// Função para reconectar Wi-Fi
void reconnectWiFi() {
  Serial.println("WiFi connection lost or failed. Reconnecting...");
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
  Serial.println("WiFi reconnected");
  Serial.println("IP address:");
  Serial.println(WiFi.localIP());
}
// Função para reconectar MQTT
void reconnectMQTT() {
  while (!client.connected()) {
    Serial.println("Connecting to MQTT broker...");
    if (client.connect("Esp32ValvulaA")) {
      Serial.println("Connected to MQTT broker");
      const char* msgvalA_status = "ESP32_Valvula";
      client.publish("valvulaA/status", msgvalA_status);  // Publica o status do ESP32 na válvula A
      client.subscribe("valvA/comando");  // Subscreve no tópico MQTT para receber comandos
    } else {
      Serial.print("Failed to connect to MQTT broker. Retrying in 5 seconds...");
      delay(5000);
    }
  }
}
// Função de retorno de chamada MQTT
void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
  }
  Serial.println();
  int command = message[0] - '0';  // Converte o primeiro caractere da mensagem em um valor inteiro
  if (command == 1) {
    openValve();  // Avança o atuador
  } else if (command == 0) {
    closeValve();  // Recua o atuador
  }
}
// Função para abrir a válvula
void openValve() {
  digitalWrite(valvA, HIGH);  // Define o pino da válvula A como HIGH (aberto)
}
// Função para fechar a válvula
void closeValve() {
  digitalWrite(valvA, LOW);  // Define o pino da válvula A como LOW (fechado)
}

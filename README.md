# MQTT_APP_Node-red
Desenvolvimento de uma aplicação em Node-Red para comunicação via MQTT com uma bancada eletropneumática.
Neste repositório é encontrado os códigos fontes para as ESP32 utilizando a Arduino IDE.

A seguir é mostrado as soluções e o vídeo de teste em laboratório LEM-04 na UFU.
## Solução 01 - Utiliza uma ESP32 para comando de válvula eletropneumática e sensores fim de curso
Link:https://www.youtube.com/watch?v=mEra2RuekDc
## Solução 02 - Utiliza duas ESP32 sendo uma para o comando da válvula eletropneumática e outra para os sensores fim de curso
Link:https://www.youtube.com/watch?v=Klcmhzi6RKE
## Conexão da ESP32 na WIFI da UFU
para conexão da ESP32 no redes sem fio da UFU bastará colocar nos campos  EAP_IDENTITY e EAP_USERNAME no código o seu usuário e no campo EAP_PASSWORD a sua senha do e-mail institucional e também o nomem da rede que está se conectando no campo ssid e assim será possível a conexão com da ESP32 com a internet dentro da rede da UFU. a figura a seguir ilustra os campos acima citados.

![image](https://github.com/MAPL-UFU/MQTT_APP_Node-red/assets/36203529/6af8e5d6-6a9a-4c3c-9406-07faeec33fd4)

Para redes externa a da UFU bastara preencher somente os campos EAP_PASSWORD e ssid e comentar os demais campos como mostra a firgura a seguir. Atenção este procedimento é válido somente para redes sem fio fora do dominio da UFU.

![image](https://github.com/MAPL-UFU/MQTT_APP_Node-red/assets/36203529/aefb1c44-8e4f-40ef-970c-4b4ba78f46ac)

Em seguida para redes externas o seguinte campo que faz a requisição de conexão da rede sem fio na ESP32  deverá ser como mostrado na figura adiante.

![image](https://github.com/MAPL-UFU/MQTT_APP_Node-red/assets/36203529/48209cf6-796d-4d27-88c2-474c562158fe)


A seguir encontra-se dois tutoriais para configuração do Node-RED e do Arduino IDE
## Configuração do Arduino IDE para ESP32
Link:https://www.youtube.com/watch?v=o6auxt4P00U&t=919s
## Instalação e Configuração do Node-RED
Link:https://www.youtube.com/watch?v=Ae4olYvJ6Zw


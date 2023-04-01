/*
 * DCCpp WebSocketServer para DCCpp Android Cab
 * Controla DCC con un servidor WebScocket 
 * En el monitor serial nos dara la IP asignada por el 
 * router y conectamos con ws://192.168.1.X
 * Esta opción es muy útil para poder conectar 
 * DCCpp Android Cab a DCC++ original. 
 * También puede usarse para conectar otros dispositivos
 * que ulicen el mismo protocolo de comunicación 
 * Autor: Carlos Muñoz
 * http://lamaquetade.infotronikblog.com
 */

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WebSocketsServer.h>
#define PUERTO 2560

WebSocketsServer webSocket = WebSocketsServer(PUERTO);

// Asignar una IP fija:

// IPAddress ip(192, 168, 4, 5); // Ip
// IPAddress gateway(192, 168, 4, 1); // Puerta de acceso
// IPAddress subnet(255, 255, 255, 0); // Mascara de subred

const char* ssid     = "Network_name"; // Nombre de la red
const char* password = "network_pass";    // Contraseña de red    

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t lenght) {

  switch (type) {
    case WStype_DISCONNECTED:
      break;
    case WStype_CONNECTED:
        webSocket.sendTXT(num, "Conectado");
        Serial.println(F("Conectado"));
      break;
    case WStype_TEXT: {
        String text = String((char *) &payload[0]);
        Serial.println(text);
        break;
      }
  }
}

void setup() {

  Serial.begin(115200);
//  WiFi.config(ip, gateway, subnet);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.println(F("Reconectando.."));
    delay(500);
  }
  delay(3000);
  // Serial.println(WiFi.localIP());
  Serial.print(F("<I"));        // Comando "I" en https://github.com/Peyutron/DCCpp_LMD
  Serial.print(WiFi.localIP()); // para mostrar IP en pantalla OLED
  Serial.println(F(">"));            

  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
}

void loop() {
  webSocket.loop();
  if (Serial.available() > 0)
  {
    String datos = Serial.readString();
    webSocket.broadcastTXT(datos);
  }
}

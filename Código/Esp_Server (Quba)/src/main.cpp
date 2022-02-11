//Para subir el programa en Linux usamos el comando: 
// sudo chmod a+rw /dev/ttyUSB0

#include <Arduino.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WebSocketsServer.h>
///Pagina html index.html/// 
#include <index.h>

bool LEDonoff; 
String JSONtxt; 

/// ID ///
const char* ssid = "MEGACABLE-979F";
const char* password = "8eAYgaeY"; 

//INiciamos el sevidor en el puerto 80: 
ESP8266WebServer server(80);
WebSocketsServer webSockets = WebSocketsServer(81); 

//Manejo de peticiones: 
void handle_root(){
  server.send(200, "text/html", html); 
}

//Manejo de las peticiones de websockets: 
void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t welength)
{
  String payloadString = (const char *)payload;
  Serial.print("payloadString= ");
  Serial.println(payloadString);

  if(type == WStype_TEXT) //receive text from client
  {
    
  // Figure out the type of WebSocket event
  switch(type) {

    // Client has disconnected
    case WStype_DISCONNECTED:
      Serial.printf("[%u] Disconnected!\n", num);
      break;

    // New client has connected
    case WStype_CONNECTED:
      {
        IPAddress ip = webSockets.remoteIP(num);
        Serial.printf("[%u] Connection from ", num);
        Serial.println(ip.toString());
      }
      break;

    // Echo text message back to client
    case WStype_TEXT:
      Serial.printf("[%u] Text: %s\n", num, payload);
      webSockets.sendTXT(num, payload);
      break;

    // For everything else: do nothing
    case WStype_BIN:
    case WStype_ERROR:
    case WStype_FRAGMENT_TEXT_START:
    case WStype_FRAGMENT_BIN_START:
    case WStype_FRAGMENT:
    case WStype_FRAGMENT_FIN:
    default:
      break;
  }
  }
}

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, password); 
  Serial.print("\n\r ....");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(200);
    Serial.print("No connected");
  }

  Serial.println("Connected to wifi"); 
  Serial.println("ip = ");
  Serial.println(WiFi.localIP()); 

  /////// Manejo de las req: /////
  server.on("/", handle_root); 
  webSockets.onEvent(webSocketEvent);
  
  /// Inicio de los servidores ////
  server.begin();
  webSockets.begin();
  Serial.println("Server start"); 
}

void loop() {
  server.handleClient();
  webSockets.loop();
}
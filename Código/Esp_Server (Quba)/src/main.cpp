#include <Arduino.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WebSocketsServer.h>
#include <string>

///Pagina html index.html/// 
#include <index.h>
#include <submit.h>
#include <error.h>

bool LEDonoff; 
bool play = false;  
bool toggle = false; 

// Nombre y contrasena de al red: 
char str[30] ; 
//Parametros de red: 
String parametros_red[] = {"",""}; 
String ip ;

IPAddress ipStatic(192,168,100,82);
IPAddress ipGateway(192,168,0,1);
IPAddress subnet(255,255,255,0); 

//Contador: 
int i = 0; 
 

/// ID ///
 const char* ssid = "EspAP";
// const char* password = "8eAYgaeY"; 

//Iniciamos el sevidor en el puerto 80: 
ESP8266WebServer server(80);
WebSocketsServer webSockets = WebSocketsServer(81); 

/// Funciones: ////
bool toggle_button(bool toggle){
    if (toggle == false){ 
      return true; 
    }else{
      return false;  
    }
}


//Manejo de peticiones http: 
void handle_root(){
  if (ip == "192.168.4.1"){
    server.send(200, "text/html", submit_html); 
  }else{
    server.send(200,"text/html", html);
  }
}

void control_root(){
   server.send(200,"text/html", html);
}

void error_root(){
  server.send(200, "text/html", error);
}

//Manejo de las peticiones de websockets: 
void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t welength)
{
  String payloadString = (char *)payload;
  
  if(type == WStype_TEXT) 
  {
  // Que tipo de conexion es: 
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
    {
      Serial.printf("[%u] Text: %s\n", num, payload);
      // play = toggle_button(toggle);
      // toggle = toggle_button(toggle); 

      for (int i = 0; i < payloadString.length(); i++)
      {
        str[i] = {payloadString[i]};
      }  
      char* nombre = strtok(str, ","); 
      char* pasword = strtok(NULL, " "); 
      Serial.printf("Nombre red: %s, Contrasena: %s \n", nombre, pasword); 
      webSockets.sendTXT(num, "Conectando...");

      // Conexion a la red :
      WiFi.config(ipStatic, ipGateway, subnet);
      WiFi.begin(nombre, pasword); 
      Serial.print("\n\r ....");
      

      int time_finish = 0; 
      unsigned long int actual_time = millis();
      unsigned long int prev_time = millis();
      while ((WiFi.status() != WL_CONNECTED) && (time_finish != 1))
      { 
        actual_time = millis();
        delay(200);
        Serial.println("No connected");
        if(actual_time - prev_time > 15000){
          prev_time = actual_time;
          time_finish = 1;
          Serial.printf("Acabo el tiempo \n");  
          WiFi.disconnect(); 
          setup(); 
          loop();
        } 
      }
      
      if(WiFi.status() == WL_CONNECTED){
        Serial.println("Connected to wifi"); 
        Serial.println("ip = ");
        Serial.println(WiFi.localIP()); 
        IPAddress miIp = WiFi.localIP();
        ip = miIp.toString(); 
      }
    }
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

  WiFi.softAP(ssid); 
  IPAddress miIP = WiFi.softAPIP(); //IP por default: 192.168.4.1  
  ip = miIP.toString();
  Serial.println("IP del APoint"); 
  Serial.println(miIP);
  Serial.println(WiFi.localIP()); 

  /////// Manejo de las req: /////
  
    Serial.println("Hndle_server");
    server.on("/", handle_root); 
    server.on("/control", control_root); 
    server.on("/error", error_root); 
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
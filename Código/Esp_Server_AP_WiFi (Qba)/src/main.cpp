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

//Led de prueba: 
#define D4 2

//pasword de mi internet xd= 8eAYgaeY

//Parametros del bot
bool LEDonoff; 
bool play = false;  
bool toggle = false; 

//Parametros de red: 
String ip ;
IPAddress ipStatic(192,168,100,82);
IPAddress ipGateway(192,168,0,1);
IPAddress subnet(255,255,255,0); 
const char* ssid = "EspAP"; //Nombre del punto de acceso.
char* password;
char* nombre; 

//Contadores: 
int i = 0; 

//Iniciamos el sevidor en el puerto 80: 
ESP8266WebServer server(80);
WebSocketsServer webSockets = WebSocketsServer(81); 

/// Funciones: ////

bool connected_to_wifi(char* name, char* pass){
  Serial.printf("nombre: %s",name); 
  WiFi.config(ipStatic, ipGateway, subnet);
  WiFi.begin(name, pass); 
  Serial.print("\n\r ....");
  int time_finish = 0; 
  unsigned long int actual_time = millis();
  unsigned long int prev_time = millis();
  while ((WiFi.status() != WL_CONNECTED) && (time_finish != 1)){ 
    actual_time = millis();
    delay(200);
    Serial.println("No connected");
    if(actual_time - prev_time > 15000){
      prev_time = actual_time;
      time_finish = 1;
      Serial.println("Acabo el tiempo"); 
      return false;
    } 
  }return true;
}

void AP_mode(){
  WiFi.softAP(ssid); 
  IPAddress miIP = WiFi.softAPIP(); //IP por default: 192.168.4.1  
  ip = miIP.toString();
  Serial.println("IP del APoint"); 
  Serial.println(miIP);
  Serial.println(WiFi.localIP()); 
}


//Manejo de peticiones http: 
//Interfaz de control: 
void control_root(){
   server.send(200,"text/html", html);
}
void handle_root(){
  String ip_AP = "192.168.4.1"; 
  if(ip == ip_AP){
    server.send(200, "text/html", submit_html);  
    Serial.println("Submit");
  }else{ 
    server.send(200,"text/html", html);
    Serial.println("Control root"); 
  }
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
      char str[30]; //Usado para dividir cadena de entrada. 
      Serial.printf("[%u] Text: %s\n", num, payload);
      
      for (int i = 0; i < payloadString.length(); i++)
      {
        str[i] = {payloadString[i]};
      }  

      nombre = strtok(str, ","); 
      password = strtok(NULL, ""); 

      //Intenta conectarse a la red: 
      Serial.printf("nombre: %s, contraseña: %s",nombre, password); 
      WiFi.config(ipStatic, ipGateway, subnet);
      WiFi.begin(nombre, password); 
      Serial.print("\n\r ....");
      int time_finish = 0; 
      unsigned long int actual_time = millis();
      unsigned long int prev_time = millis();

      while ((WiFi.status() != WL_CONNECTED) && (time_finish != 1)){ 
      actual_time = millis();
      delay(200);
      Serial.println("No connected");
      if(actual_time - prev_time > 13000){
        prev_time = actual_time;
        time_finish = 1;
        Serial.println("Acabo el tiempo"); 
        } 
      }
      if(WiFi.status() != WL_CONNECTED){
        Serial.println("No se pudo conectar"); 
        WiFi.disconnect(); 

      }else{
        Serial.println("Wifi conectada en la ip 192.168.100.82");  
        ip = "192.168.100.82"; 
        digitalWrite(D2,LOW); 
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

  WiFiMode(WIFI_AP_STA);
  //Intenta conectarse a una red pasada (AUN NO ESTA)
  if(nombre != NULL){
    if(connected_to_wifi(nombre, password) != true){
      Serial.println("No se pudo conectar inicio");
    }
  }
  
  AP_mode();
  /////// Manejo de las req: /////
  Serial.println("Hadle_server");
  server.on("/", handle_root); 
  server.on("/index", control_root); 
  server.on("/error", error_root); 

  //Leds indicadores: 
  pinMode(D2, OUTPUT);

  /// Inicio de los servidores ////
  webSockets.onEvent(webSocketEvent); 
  server.begin();
  webSockets.begin();
  Serial.println("Server start"); 
  
}

void loop() {
  server.handleClient();
  webSockets.loop();  
}
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

//Parametros del bot
bool play = false;  
int range = 0; //Valor del slider: de 0 a 100

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
bool toggle_button(String payload){
  //Cambia el valor del estado: 
  if (payload == "play")
        {
          return true;
        }else{
          return false;
        }
}
bool connected_to_wifi(char* name, char* pass){
  //Devuelve true si se pudo conectar: 

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
  //Crea un AP: 
  WiFi.softAP(ssid); 
  IPAddress miIP = WiFi.softAPIP(); //IP por default: 192.168.4.1  
  ip = miIP.toString();
  Serial.println("IP del APoint"); 
  Serial.println(miIP);
  Serial.println(WiFi.localIP()); 
}


//Manejo de peticiones http:  
void control_root(){
  //Interfaz de control:
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
      Serial.printf("[%u] Text: %s\n", num, payload);
      char str1[30]; //Usado para dividir cadena de entrada del websocket. 
      for (int i = 0; i < payloadString.length(); i++)
      {
        str1[i] = {payloadString[i]};
      }  
      char* payloadString_first_position = strtok(str1, ",");
      char* payloadString_second_position = strtok(NULL, "");

      String payloadStringfirst = (char *)payloadString_first_position; 
      String payloadStringsecond = (char *)payloadString_second_position;       
      Serial.printf("payloadString_first_position: %s \n", payloadString_first_position); 
      
      //Verifica si son valores del slider: 
      if(payloadStringfirst == "range"){
        range = payloadStringsecond.toInt();
        Serial.printf("Range: %i \n", range); 

      //Si es play o stop: 
      }else if (payloadString == "play" || payloadString == "stop"){
        play = toggle_button(payloadString);
        Serial.println(play); 
      //Si son credenciales de internet: 
      }else{
        char str[30]; //Usado para dividir cadena de entrada del websocket. 
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
        //No se pudo conectar: 
        if(WiFi.status() != WL_CONNECTED){
          Serial.println("No se pudo conectar"); 
          WiFi.disconnect(); 
        }
        //Se conecto: 
        else{
          Serial.println("Wifi conectada en la ip 192.168.100.82");  
          ip = "192.168.100.82"; 
          digitalWrite(D4,HIGH); 
        }
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
  pinMode(D4, OUTPUT);

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
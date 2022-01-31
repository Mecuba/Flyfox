// 4-channel RC receiver for controlling
// an RC car / boat / plane / quadcopter / etc.
// using an ESP8266 and an Android phone with RoboRemo app


#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <Servo.h>

// config:
const char* ssidSTA1     = "Habia una vez";
const char* passwordSTA1 = "-truz, jaja";
const char* ssidSTA2     = "Inventoteca_2G";
const char* passwordSTA2 = "science_7425";

const char *ssid = "Deslizainador";  // You will connect your phone to this Access Point
const char *pw = "picaporte"; // and this is the password
IPAddress ip(192, 168, 0, 1); // From RoboRemo app, connect to this IP
IPAddress netmask(255, 255, 255, 0);
const int port = 9876; // and this port

const int chCount = 4; // 4 channels, you can add more if you have GPIOs :)
Servo servoCh[chCount]; // will generate 4 servo PWM signals
int chPin[] = {0, 2, 14, 12}; // ESP pins: GPIO 0, 2, 14, 12
int chVal[] = {1500, 1500, 1500, 1500}; // default value (middle)

int usMin = 700; // min pulse width
int usMax = 2300; // max pulse width


WiFiServer server(80);
WiFiClient client;


char cmd[100]; // stores the command chars received from RoboRemo
int cmdIndex;
unsigned long lastCmdTime = 60000;
unsigned long aliveSentTime = 0;
int WifiTimeout = 15000;

char ipAd[100];


boolean cmdStartsWith(const char *st) { // checks if cmd starts with st
  for(int i=0; ; i++) {
    if(st[i]==0) return true;
    if(cmd[i]==0) return false;
    if(cmd[i]!=st[i]) return false;;
  }
  return false;
}


void exeCmd() { // executes the command from cmd

  lastCmdTime = millis();
  digitalWrite(LED_BUILTIN, LOW);
  
  if( cmdStartsWith("ch") ) { // Set RoboRemo slider id to "ch0", set min 1000 and set max 2000
    int ch = cmd[2] - '0';
    if(ch>=0 && ch<=9 && cmd[3]==' ') {
      chVal[ch] = (int)atof(cmd+4); 
      servoCh[ch].writeMicroseconds(chVal[ch]);
      client.write("Ms ");
      client.write(cmd+4);
      client.write(" \n");
    }
  }

  if( cmdStartsWith("st") ) { // Set RoboRemo slider id to "ch0", set min 1000 and set max 2000
    int ch = cmd[2] - '0';
    if(ch>=0 && ch<=9 && cmd[3]==' ') {
      chVal[ch] = (int)atof(cmd+4);

      Serial.println(chVal[ch]);
      if(chVal[ch] == 1) servoCh[0].attach(chPin[0], usMin, usMax);
      else if(chVal[ch] == 0) servoCh[0].detach();
      
      client.write("ServoOn ");
      client.write(cmd+4);
      client.write(" \n");
    }
  }
  
  // use accelerometer: set RoboRemo acc y id to "ca1"
  
  if( cmdStartsWith("ca") ) {
    int ch = cmd[2] - '0';
    if(ch>=0 && ch<=9 && cmd[3]==' ') {
      chVal[ch] = (usMax+usMin)/2 + (int)( atof(cmd+4)*51 ); // 9.8*51 = 500 => 1000 .. 2000
      if(!servoCh[ch].attached()) {
        servoCh[ch].attach(chPin[ch], usMin, usMax);
      }   
      servoCh[ch].writeMicroseconds(chVal[ch]);
    }
  }
  
}

void setup(){
  
  cmdIndex = 0;

  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  delay(100);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(3000);
  digitalWrite(LED_BUILTIN, LOW);
  
  WiFi.softAPConfig(ip, ip, netmask); // configure ip address for softAP 
  WiFi.softAP(ssid, pw); // configure ssid and password for softAP
  server.begin(); // start TCP server
  Serial.println((String)"SSID: " + ssid + "  PASS: " + pw);
  Serial.println((String)"RoboRemo app must connect to " + ip.toString() + ":" + port);
  Serial.println("");

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssidSTA1);
  WiFi.begin(ssidSTA1, passwordSTA1);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if(millis()>=WifiTimeout) {
      Serial.println("Failed to connect to STA1");
      Serial.print("Connecting to ");
      Serial.println(ssidSTA2);
      WiFi.begin(ssidSTA2, passwordSTA2);
      while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
        if(millis()>=WifiTimeout*2) {
          Serial.println("Failed to connect to STA2");
          Serial.println("Vamonos al void loop().");
          return;
        }
      }
    }
  }
  // Print local IP address
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
  
  delay(500);
}


void loop() {
  digitalWrite(LED_BUILTIN, LOW);
  if(millis() - lastCmdTime > 5000) {  // if contact lost for more than half second
//    for(int i=0; i<chCount; i++) {
//      // set all values to middle
//      servoCh[i].writeMicroseconds( (usMin + usMax)/2 );
//      servoCh[i].detach(); // stop PWM signals
//    }
  }

  if(!client.connected()) {
    client = server.available();
    return;
  }

  if(client.available()) {
    digitalWrite(LED_BUILTIN, HIGH);
    char c = (char)client.read(); // read char from client (RoboRemo app)
    if(c=='\n') { // if it is command ending
      cmd[cmdIndex] = 0;
      exeCmd();  // execute the command
      cmdIndex = 0; // reset the cmdIndex
    } else {      
      cmd[cmdIndex] = c; // add to the cmd buffer
      if(cmdIndex<99) cmdIndex++;
    }
  }
   

  if(millis() - aliveSentTime > 500) { // every 500ms 
    client.write("alive 1\n"); // send the alive signal, so the "connected" LED in RoboRemo will stay ON (the LED must have the id set to "alive")
    aliveSentTime = millis();
    // and the LED will turn off (because it has the "on timeout" set to 700 (ms) )
  }

}

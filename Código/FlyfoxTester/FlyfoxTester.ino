/* FlyfoxTester
  Programa para probar el Flyfox con torreta escrito a la 1:23 am. 
*/

#include <Servo.h>

Servo ServoSlide;  // create servo object to control a servo
Servo ServoPan; 
Servo ServoTilt; 


void setup() {

  Serial.begin(115200);
  delay(500);
  Serial.println("Encendiendo Flyfox...");
  
  ServoSlide.attach(D3);  // attaches the servo on GIO2 to the servo object
  ServoPan.attach(D2);
  ServoTilt.attach(D1);

  Serial.println("Quieto y pos inicial.");
  ServoSlide.write(161); //Pos de quieto para Slide  
  delay(3000);  //Delay de seguridad
  ServoPan.write(90); //Pos media            
  delay(3000);  //Delay de seguridad
  ServoTilt.write(90); //Pos media
  delay(3000);  //Delay de seguridad
}

void loop() {

    //ADELANTE
    ServoSlide.write(180); //Pos adelante para Slide
    Serial.println("Slide: 180");
    delay(500);  
    ServoPan.write(180); //Pos max 180
    Serial.println("Pan: 180");
    delay(500);              
    ServoTilt.write(180); //Pos max 180
    Serial.println("Tilt: 180");
    delay(1000); 

    //MEDIO
    ServoSlide.write(161); //Pos de quieto para Slide
    Serial.println("Slide: 161");
    delay(500);  
    ServoPan.write(90); //Pos media
    Serial.println("Pan: 90");
    delay(500);              
    ServoTilt.write(90); //Pos media
    Serial.println("Tilt: 90");
    delay(1000);

    //ATRÁS
    ServoSlide.write(144); //Pos de atras para Slide
    Serial.println("Slide: 144");
    delay(500);  
    ServoPan.write(0); //Pos max 0
    Serial.println("Pan: 0");
    delay(500);              
    ServoTilt.write(0); //Pos max 0
    Serial.println("Tilt: 0");
    delay(1000);

    //MEDIO
    ServoSlide.write(161); //Pos de quieto para Slide
    Serial.println("Slide: 161");
    delay(500);  
    ServoPan.write(90); //Pos media
    Serial.println("Pan: 90");
    delay(500);              
    ServoTilt.write(90); //Pos media
    Serial.println("Tilt: 90");
    delay(1000);

}

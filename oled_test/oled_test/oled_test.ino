#include "Face.h"

// Using ESP32, connect to OLED:
// 3V3 - VCC
// GND - GND
// D21 - SDA
// D22 - SCL

Face face;

void setup() {
  Serial.begin(115200);

  face.initialize();
}

void loop() {

  face.drawFace(0, 0, 1000);
 // face.doLaugh();

   face.drawFace(-1,0, 2000);
    face.blinkEyes(-1,0);

    face.drawFace(0,0, 2000);
  
    face.drawFace(-1,0, 1000);
    face.drawFace(0,0, 2000);
    face.drawFace(2,0, 2000);
    face.drawFace(2,-2, 1000);
  
    face.blinkEyes(2,-2);
  
    face.drawFace(0,0, 2000);
    face.drawFace(1,0, 2000);
  
    face.blinkEyes(1,0);
  
    face.drawFace(-2,0,2000);
    face.drawFace(0,0, 2000);
    face.drawFace(0,-2, 1000);
    face.drawFace(0,0, 2000);
    face.drawFace(-2,0, 1000);
  
    face.blinkEyes(-1,0);
  
    face.drawFace(0,0, 2000);
    face.drawFace(-2,0, 3000);
}

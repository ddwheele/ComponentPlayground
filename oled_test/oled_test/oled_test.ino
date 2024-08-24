#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16

#define EYE_RADIUS 15
#define EYE_X_INSET 5 // side of screen to outside edge of eye
#define EYE_Y_INSET 5 // top of screen to top edge of eye
//#define PUPIL_RADIUS 5

#define MOUTH_RADIUS 15
#define MOUTH_OFFSET 6 // Bottom of screen to bottom of smile
#define MOUTH_HEIGHT 4 // Bottom of smile to top of smile

// Using ESP32, connect to OLED:
// 3V3 - VCC
// GND - GND
// D21 - SDA
// D22 - SCL

void setup() {
  Serial.begin(115200);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(1000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();
 
}

void loop() {
  drawFace(0,0, 1000);
  drawFace(1,0, 2000);
  
  blinkEyes(1,0);
  
  drawFace(0,0, 2000);
  drawFace(-1,0, 1000);
  drawFace(0,0, 2000);
  drawFace(2,0, 2000);
  drawFace(2,-2, 1000);

  blinkEyes(2,-2);

  drawFace(0,0, 2000);
  drawFace(1,0, 2000);
  
  blinkEyes(1,0);

  drawFace(-2,0,2000);
  drawFace(0,0, 2000);
  drawFace(0,-2, 1000);
  drawFace(0,0, 2000);
  drawFace(-2,0, 1000);
 
  blinkEyes(1,0);

  drawFace(0,0, 2000);
  drawFace(-2,0, 3000);
}

void blinkEyes(int left, int up) {
  for(int i=0; i<=100; i+=33) {
    drawBlinkStep(left, up, i, 10);
  }
  for(int i=100; i>0; i-=33) {
    drawBlinkStep(left, up, i, 10);
  }
}

void drawBlinkStep(int left, int up, int percentClosed, int hold) {
  int screenCenterX = display.width() / 2;
  int screenCenterY = display.height() / 2;

  int leftEyeCenterX = calcLeftEyeCenterX(left, up);
  int rightEyeCenterX = calcRightEyeCenterX(left, up);
  int eyeCenterY = calcEyeCenterY(up);
  
  drawEyes(left, up, leftEyeCenterX, rightEyeCenterX, eyeCenterY);

  // Open, the lid is centered at -2R. Closed, centered at 0.
  int lidCenterY = map(percentClosed, 0, 100, -2*EYE_RADIUS, 0); 
  // draw "lid"
  display.fillCircle(leftEyeCenterX,
                     lidCenterY,
                     EYE_RADIUS*2,
                     SSD1306_BLACK);
                     
  display.fillCircle(rightEyeCenterX,
                     lidCenterY,
                     EYE_RADIUS*2,
                     SSD1306_BLACK);
  display.display();
  delay(hold);
}

void drawFace(int left, int up, int hold) {
   display.clearDisplay();
   drawMouth();
   drawEyes(left, up);

   display.display();
   delay(hold);
}

int calcMouthBoxY() {
  return display.height() - (MOUTH_OFFSET+MOUTH_HEIGHT);
}

void drawMouth() {
  int screenCenterX = display.width() / 2;
  int mouthCenterY = display.height() - MOUTH_OFFSET - MOUTH_RADIUS;

  display.drawCircle(screenCenterX, mouthCenterY, MOUTH_RADIUS, SSD1306_WHITE);
  display.fillRect(0,0,
                   display.width(), calcMouthBoxY(),
                   SSD1306_BLACK);
}

/**
 * Draw "standard" eyes, shifted left and up pixels in those directions
 * (left and up can be negative)
 */
void drawEyes(int left, int up) {
  int leftEyeCenterX = calcLeftEyeCenterX(left, up);
  int rightEyeCenterX = calcRightEyeCenterX(left, up);
  int eyeCenterY = calcEyeCenterY(up);

  drawEyes(left, up, leftEyeCenterX, rightEyeCenterX, eyeCenterY);
}

void drawEyes(int left, int up, int leftEyeCenterX, int rightEyeCenterX, int eyeCenterY) {
  int screenCenterX = display.width() / 2;
  int screenCenterY = display.height() / 2;

  // draw white
  display.fillCircle(leftEyeCenterX,
                     eyeCenterY,
                     EYE_RADIUS,
                     SSD1306_WHITE);
                     
  display.fillCircle(rightEyeCenterX,
                     eyeCenterY,
                     EYE_RADIUS,
                     SSD1306_WHITE);

#ifdef PUPIL_RADIUS
  // draw pupil
  display.fillCircle(leftEyeCenterX,
                     eyeCenterY,
                     PUPIL_RADIUS,
                     SSD1306_INVERSE);
                     
  display.fillCircle(rightEyeCenterX,
                     eyeCenterY,
                     PUPIL_RADIUS,
                     SSD1306_INVERSE);
#endif
}



int calcLeftEyeCenterX(int left, int up) {
  return EYE_X_INSET + EYE_RADIUS - left;
}

int calcRightEyeCenterX(int left, int up) {
  return display.width() - EYE_X_INSET - EYE_RADIUS - left -1;
}

int calcEyeCenterY(int up) {
  return EYE_RADIUS + EYE_Y_INSET - up;
}

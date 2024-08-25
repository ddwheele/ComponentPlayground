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

#define EYE_RADIUS          18
#define EYE_X_INSET         5    // side of screen to outside edge of eye
#define EYE_Y_INSET         5    // top of screen to top edge of eye
#define PUPIL_RADIUS        16
#define HILITE_RADIUS    2
#define HILITE_OFFSET_X  6
#define HILITE_OFFSET_Y  6

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
 // drawFace(-1,0, 2000);  
  blinkEyes(-1,0);
  
//  drawFace(0,0, 2000);
//
//  drawFace(-1,0, 1000);
//  drawFace(0,0, 2000);
//  drawFace(2,0, 2000);
//  drawFace(2,-2, 1000);
//
//  blinkEyes(2,-2);
//
//  drawFace(0,0, 2000);
//  drawFace(1,0, 2000);
//  
//  blinkEyes(1,0);
//
//  drawFace(-2,0,2000);
//  drawFace(0,0, 2000);
//  drawFace(0,-2, 1000);
//  drawFace(0,0, 2000);
//  drawFace(-2,0, 1000);
// 
//  blinkEyes(-1,0);
//
//  drawFace(0,0, 2000);
//  drawFace(-2,0, 3000);
}

void drawFace(int left, int up, int hold) {
  display.clearDisplay();
  drawSmileMouth();
  drawEyes(left, up);

  //display.invertDisplay(true);
  display.display();
  delay(hold);
}

void blinkEyes(int left, int up) {
  for(int i=0; i<=100; i+=33) { //33 was good
    drawBlinkStep(left, up, i, 10); // 10 was good
  }
  for(int i=100; i>0; i-=33) {
    drawBlinkStep(left, up, i, 10);
  }
}

void drawBlinkStep(int left, int up, int percentClosed, int hold) {
  int leftEyeCenterX = calcLeftEyeCenterX(left, up);
  int rightEyeCenterX = calcRightEyeCenterX(left, up);
  int eyeCenterY = calcEyeCenterY(up);

  drawSmileMouth();
  drawEyes(left, up, leftEyeCenterX, rightEyeCenterX, eyeCenterY);

  int lidCenterY = map(percentClosed, 0, 100,
                       calcLidCenterOpen(up, eyeCenterY),
                       calcLidCenterClosed(up, eyeCenterY)); 

  // draw "lid" (doubled line)
  display.fillCircle(leftEyeCenterX,
                     lidCenterY,
                     EYE_RADIUS,
                     SSD1306_WHITE);

  display.fillCircle(leftEyeCenterX,
                     lidCenterY,
                     EYE_RADIUS-2,
                     SSD1306_BLACK);
                     
  display.fillCircle(rightEyeCenterX,
                     lidCenterY,
                     EYE_RADIUS,
                     SSD1306_WHITE);
                     
  display.fillCircle(rightEyeCenterX,
                     lidCenterY,
                     EYE_RADIUS-2,
                     SSD1306_BLACK);
                     
  // "erase" top part           
  display.fillRect(0,0,
                   display.width(), (lidCenterY+eyeCenterY)/2,
                   SSD1306_BLACK);
  display.display();
  delay(hold);
}

void laughEyes() {
  int leftEyeCenterX = calcLeftEyeCenterX(left, up);
  int rightEyeCenterX = calcRightEyeCenterX(left, up);
  int eyeCenterY = calcEyeCenterY(up);

  drawSmileMouth();
  drawEyes(left, up, leftEyeCenterX, rightEyeCenterX, eyeCenterY);

  int lidCenterY = map(percentClosed, 0, 100,
                       calcLidCenterOpen(up, eyeCenterY),
                       calcLidCenterClosed(up, eyeCenterY)); 

  // draw "lid" (doubled line)
  display.fillCircle(leftEyeCenterX,
                     lidCenterY,
                     EYE_RADIUS,
                     SSD1306_WHITE);

  display.fillCircle(leftEyeCenterX,
                     lidCenterY,
                     EYE_RADIUS-2,
                     SSD1306_BLACK);
                     
  display.fillCircle(rightEyeCenterX,
                     lidCenterY,
                     EYE_RADIUS,
                     SSD1306_WHITE);
                     
  display.fillCircle(rightEyeCenterX,
                     lidCenterY,
                     EYE_RADIUS-2,
                     SSD1306_BLACK);
                     
  // "erase" top part           
  display.fillRect(0,0,
                   display.width(), (lidCenterY+eyeCenterY)/2,
                   SSD1306_BLACK);
  display.display();
  delay(hold);
}

void drawLaughEyes() {
  int leftEyeCenterX = calcLeftEyeCenterX(0, 0);
  int rightEyeCenterX = calcRightEyeCenterX(0, 0);
  int eyeCenterY = calcEyeCenterY(0);

  
}

void drawSmileMouth() {
  int screenCenterX = display.width() / 2;
  int mouthCenterY = display.height() - MOUTH_OFFSET - MOUTH_RADIUS;

  // doubled line
  display.drawCircle(screenCenterX, mouthCenterY, MOUTH_RADIUS, SSD1306_WHITE);
  display.drawCircle(screenCenterX, mouthCenterY, MOUTH_RADIUS-1, SSD1306_WHITE);

  // erase top part
  display.fillRect(0,0,
                   display.width(), calcMouthBoxY(),
                   SSD1306_BLACK);
}

void drawOooMouth() {
  int screenCenterX = display.width() / 2;
  int oooRadius = MOUTH_HEIGHT * 1.5;
  int mouthCenterY = display.height() - MOUTH_OFFSET - oooRadius;

  display.drawCircle(screenCenterX, mouthCenterY, oooRadius, SSD1306_WHITE);
  display.drawCircle(screenCenterX, mouthCenterY, oooRadius-1, SSD1306_WHITE);
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
  // draw white
  display.fillCircle(leftEyeCenterX,
                     eyeCenterY,
                     EYE_RADIUS,
                     SSD1306_WHITE);
                     
  display.fillCircle(rightEyeCenterX,
                     eyeCenterY,
                     EYE_RADIUS,
                     SSD1306_WHITE);

  // draw pupil
  display.fillCircle(leftEyeCenterX,
                     eyeCenterY,
                     PUPIL_RADIUS,
                     SSD1306_INVERSE);
                     
  display.fillCircle(rightEyeCenterX,
                     eyeCenterY,
                     PUPIL_RADIUS,
                     SSD1306_INVERSE);

  // draw Highlights
  display.fillCircle(calcLeftEyeHighlightX(left, up),
                     calcEyeHighlightY(up),
                     HILITE_RADIUS,
                     SSD1306_INVERSE);

  display.fillCircle(calcRightEyeHighlightX(left, up),
                     calcEyeHighlightY(up),
                     HILITE_RADIUS,
                     SSD1306_INVERSE);
}

int calcMouthBoxY() {
  return display.height() - (MOUTH_OFFSET+MOUTH_HEIGHT);
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

int calcLeftEyeHighlightX(int left, int up) {
  int eyeCenter = calcLeftEyeCenterX(left, up);
  return eyeCenter + HILITE_OFFSET_X;
}

int calcRightEyeHighlightX(int left, int up) {
  int eyeCenter = calcRightEyeCenterX(left, up);
  return eyeCenter + HILITE_OFFSET_X;
}

int calcEyeHighlightY(int up) {
  int eyeCenter = calcEyeCenterY(up);
  return eyeCenter - HILITE_OFFSET_Y;
}

int calcLidCenterOpen(int up, int eyeCenterY) {
  return eyeCenterY - 2 * EYE_RADIUS;
}

int calcLidCenterClosed(int up, int eyeCenterY) {
  return eyeCenterY;
}

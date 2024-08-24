#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define NUMFLAKES     10 // Number of snowflakes in the animation example

#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16

#define EYE_RADIUS 23
#define EYE_OFFSET 25
#define PUPIL_RADIUS 5

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
  delay(2000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();
 
}

void loop() {
  drawEyes(0,0, 1000);
  drawEyes(1,0, 2000);
  blinkEyes(1,0);
  drawEyes(0,0, 2000);
  drawEyes(-1,0, 1000);

  drawEyes(0,0, 2000);
  drawEyes(2,0, 2000);
  drawEyes(2,-2, 1000);
  blinkEyes(2,-2);
  drawEyes(0,0, 2000);

  drawEyes(1,0, 2000);
  blinkEyes(1,0);
  drawEyes(-2,0,2000);
  drawEyes(0,0, 2000);
  drawEyes(0,-2, 1000);

  drawEyes(0,0, 2000);
  drawEyes(-2,0, 1000);
  drawEyes(0,0, 2000);
  blinkEyes(1,0);
  drawEyes(-2,0, 3000);
}

/**
 * Draw "standard" eyes, moved left and up pixels in those directions
 * (left and up can be negative)
 */
void drawEyes(int left, int up, int hold) {
  int screenCenterX = display.width() / 2;
  int screenCenterY = display.height() / 2;

  int leftEyeCenterX = EYE_OFFSET - left;
  int rightEyeCenterX = display.width() - EYE_OFFSET - left -1;
  int eyeCenterY = EYE_OFFSET - up;
  
  display.clearDisplay();
  
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
  display.display();
  delay(hold);
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

  int leftEyeCenterX = EYE_OFFSET - left;
  int rightEyeCenterX = display.width() - EYE_OFFSET - left -1;
  int eyeCenterY = EYE_OFFSET - up;
  
  display.clearDisplay();
  
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


#define XPOS   0 // Indexes into the 'icons' array in function below
#define YPOS   1
#define DELTAY 2

void testanimate(const uint8_t *bitmap, uint8_t w, uint8_t h) {
  int8_t f, icons[NUMFLAKES][3];

  // Initialize 'snowflake' positions
  for(f=0; f< NUMFLAKES; f++) {
    icons[f][XPOS]   = random(1 - LOGO_WIDTH, display.width());
    icons[f][YPOS]   = -LOGO_HEIGHT;
    icons[f][DELTAY] = random(1, 6);
    Serial.print(F("x: "));
    Serial.print(icons[f][XPOS], DEC);
    Serial.print(F(" y: "));
    Serial.print(icons[f][YPOS], DEC);
    Serial.print(F(" dy: "));
    Serial.println(icons[f][DELTAY], DEC);
  }

  for(;;) { // Loop forever...
    display.clearDisplay(); // Clear the display buffer

    // Draw each snowflake:
    for(f=0; f< NUMFLAKES; f++) {
      display.drawBitmap(icons[f][XPOS], icons[f][YPOS], bitmap, w, h, SSD1306_WHITE);
    }

    display.display(); // Show the display buffer on the screen
    delay(200);        // Pause for 1/10 second

    // Then update coordinates of each flake...
    for(f=0; f< NUMFLAKES; f++) {
      icons[f][YPOS] += icons[f][DELTAY];
      // If snowflake is off the bottom of the screen...
      if (icons[f][YPOS] >= display.height()) {
        // Reinitialize to a random position, just off the top
        icons[f][XPOS]   = random(1 - LOGO_WIDTH, display.width());
        icons[f][YPOS]   = -LOGO_HEIGHT;
        icons[f][DELTAY] = random(1, 6);
      }
    }
  }
}

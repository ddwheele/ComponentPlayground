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

static const unsigned char PROGMEM logo_bmp[] =
{ 0b00000000, 0b11000000,
  0b00000001, 0b11000000,
  0b00000001, 0b11000000,
  0b00000011, 0b11100000,
  0b11110011, 0b11100000,
  0b11111110, 0b11111000,
  0b01111110, 0b11111111,
  0b00110011, 0b10011111,
  0b00011111, 0b11111100,
  0b00001101, 0b01110000,
  0b00011011, 0b10100000,
  0b00111111, 0b11100000,
  0b00111111, 0b11110000,
  0b01111100, 0b11110000,
  0b01110000, 0b01110000,
  0b00000000, 0b00110000 };

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

  // Draw a single pixel in white
  display.drawPixel(10, 10, SSD1306_WHITE);

  display.display();
  delay(2000);
 
//  // Invert and restore display, pausing in-between
//  display.invertDisplay(true);
//  delay(1000);
//  display.invertDisplay(false);
//  delay(1000);
//  
//  testdrawbitmap();    // Draw a small bitmap image
//  testanimate(logo_bmp, LOGO_WIDTH, LOGO_HEIGHT); // Animate bitmaps
}

void loop() {
  drawEyes(0,0, 1000);
  drawEyes(1,0, 2000);
  display.clearDisplay();
    delay(200);
   
  drawEyes(0,0, 2000);
  drawEyes(-1,0, 1000);

  drawEyes(0,0, 2000);
  drawEyes(2,0, 2000);
  drawEyes(2,-2, 1000);
  display.clearDisplay();
  display.display();
    delay(200);
  drawEyes(0,0, 2000);

  drawEyes(1,0, 2000);
    display.clearDisplay();
    delay(200);
  drawEyes(-2,0,2000);
  drawEyes(0,0, 2000);
  drawEyes(0,-2, 1000);

  drawEyes(0,0, 2000);
  drawEyes(-2,0, 1000);
  drawEyes(0,0, 2000);
   display.clearDisplay();
  display.display();
    delay(200);
  drawEyes(-2,0, 3000);
}


/**
 * Draw "standard" eyes, moved left and up pixels in those directions
 * (left and up can be negative)
 */
void drawEyes(int left, int up, int hold) {
  int screenCenterX = display.width() / 2;
  int screenCenterY = display.height() / 2;
  
  display.clearDisplay();
  display.fillCircle(EYE_OFFSET - left,
                     EYE_OFFSET - up,
                     EYE_RADIUS,
                     SSD1306_WHITE);
  display.fillCircle(display.width() - EYE_OFFSET - left,
                     EYE_OFFSET - up,
                     EYE_RADIUS,
                     SSD1306_WHITE);

  display.display();
  delay(hold);
}

/**
 * eyeHOffset - distance from left of screen across to center of left eye
 * eyeVOffset - distance from top of screen down to center of both eyes
 * eyeRadius
 */
void eyeTest(int eyeHOffset, int eyeVOffset, int eyeRadius) {
  int screenCenterX = display.width() / 2;
  int screenCenterY = display.height() / 2;
  
  display.clearDisplay();
  display.fillCircle(eyeHOffset,
                     eyeVOffset,
                     eyeRadius,
                     SSD1306_WHITE);
  display.fillCircle(display.width() - eyeHOffset,
                     eyeVOffset,
                     eyeRadius,
                     SSD1306_WHITE);

  display.display();
  delay(3000);

  reportSizes(eyeHOffset, eyeVOffset, eyeRadius);
  delay(200);
}

void reportSizes(int eyeHOffset, int eyeVOffset, int eyeRadius) {
  display.clearDisplay();
  display.setCursor(0, 0);     // Start at top-left corner
  display.setTextSize(2);             
  display.setTextColor(SSD1306_WHITE);
  display.print(F("H=")); 
  display.print(eyeHOffset); 
  display.print(F(", V=")); 
  display.println(eyeVOffset); 
  
  display.print(F("R=")); 
  display.println(eyeRadius);

  display.display();
  delay(2000);
}

void testdrawbitmap(void) {
  display.clearDisplay();

  display.drawBitmap(
    (display.width()  - LOGO_WIDTH ) / 2,
    (display.height() - LOGO_HEIGHT) / 2,
    logo_bmp, LOGO_WIDTH, LOGO_HEIGHT, 1);
  display.display();
  delay(1000);
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

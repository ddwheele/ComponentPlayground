#ifndef FACE_H
#define FACE_H

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32


#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16

#define EYE_RADIUS       18
#define EYE_X_INSET      5    // side of screen to outside edge of eye
#define EYE_Y_INSET      5    // top of screen to top edge of eye
#define PUPIL_RADIUS     16
#define HILITE_RADIUS    2
#define HILITE_OFFSET_X  6
#define HILITE_OFFSET_Y  6

#define MOUTH_RADIUS 15  // Radius of circle used to draw mouth
#define MOUTH_OFFSET 6   // Bottom of screen to bottom of smile
#define MOUTH_HEIGHT 4   // Bottom of smile to top of smile

class Face {


  public:
    Face();

    void initialize();

    /**
     * left - translate eyes this many pixels to the left
     * up - translate eyes this many pixesl up
     * hold - ms to display this face
     */
    void drawFace(int left, int up, int hold);

    void blinkEyes(int left, int up);

    void doLaugh();    

  private:
    Adafruit_SSD1306* disp;

    void drawLaughStep(int left, int up, int hold);

    void drawBlinkStep(int left, int up, int percentClosed, int hold);

    void drawSmileMouth();

    void drawLaughingMouth();

    void drawOooMouth();

    void drawEyes(int left, int up);

    void drawLaughEyes(int left, int up, int lefeEyeCenterX, int rightEyeCenterX, int eyeCenterY);

    void drawEyes(int left, int up, int leftEyeCenterX, int rightEyeCenterX, int eyeCenterY);

    int calcTopOfMouth();

    int calcLeftEyeCenterX(int left, int up);

    int calcRightEyeCenterX(int left, int up);

    int calcEyeCenterY(int up);

    int calcLeftEyeHighlightX(int left, int up);

    int calcRightEyeHighlightX(int left, int up);

    int calcEyeHighlightY(int up);

    int calcLidCenterOpen(int up, int eyeCenterY);

    int calcLidCenterClosed(int up, int eyeCenterY);

    int calcCheekCenterSquinting(int up, int eyeCenterY);

    int calcCheekCenterNoSquinting(int up, int eyeCenterY);

};


#endif

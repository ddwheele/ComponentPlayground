#include "Face.h"

  
Face::Face() {
  // empty constructor
 disp = new Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
}

void Face::initialize() {
   // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!disp->begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  disp->display();
  delay(1000); // Pause for 2 seconds

  // Clear the buffer
  disp->clearDisplay();
}

void Face::drawFace(int left, int up, int hold) {
  disp->clearDisplay();
  drawSmileMouth();
  drawEyes(left, up);

  //disp->invertDisplay(true);
  disp->display();
  delay(hold);
}

void Face::blinkEyes(int left, int up) {
  for (int i = 0; i <= 100; i += 33) {
    drawBlinkStep(left, up, i, 10);
  }
  for (int i = 100; i > 0; i -= 33) {
    drawBlinkStep(left, up, i, 10);
  }
}

void Face::doLaugh() {
  drawLaughStep(0, -5, 500);
 
  for (int i = 0; i < 5; i++) {
    drawLaughStep(0, -3, 200);
    drawLaughStep(0, -5, 200);
  }
}

void Face::drawLaughStep(int left, int up, int hold) {
  int leftEyeCenterX = calcLeftEyeCenterX(left, up);
  int rightEyeCenterX = calcRightEyeCenterX(left, up);
  int eyeCenterY = calcEyeCenterY(up);

  drawLaughingMouth();
  drawLaughEyes(left, up, leftEyeCenterX, rightEyeCenterX, eyeCenterY);

  disp->display();
  delay(hold);
}

void Face::drawBlinkStep(int left, int up, int percentClosed, int hold) {
  int leftEyeCenterX = calcLeftEyeCenterX(left, up);
  int rightEyeCenterX = calcRightEyeCenterX(left, up);
  int eyeCenterY = calcEyeCenterY(up);

  drawSmileMouth();
  drawEyes(left, up, leftEyeCenterX, rightEyeCenterX, eyeCenterY);

  int lidCenterY = map(percentClosed, 0, 100,
                       calcLidCenterOpen(up, eyeCenterY),
                       calcLidCenterClosed(up, eyeCenterY));

  // draw "lid" (doubled line)
  disp->fillCircle(leftEyeCenterX,
                     lidCenterY,
                     EYE_RADIUS,
                     SSD1306_WHITE);

  disp->fillCircle(leftEyeCenterX,
                     lidCenterY,
                     EYE_RADIUS - 2,
                     SSD1306_BLACK);

  disp->fillCircle(rightEyeCenterX,
                     lidCenterY,
                     EYE_RADIUS,
                     SSD1306_WHITE);

  disp->fillCircle(rightEyeCenterX,
                     lidCenterY,
                     EYE_RADIUS - 2,
                     SSD1306_BLACK);

  // "erase" top part
  disp->fillRect(0, 0,
                   disp->width(), (eyeCenterY + lidCenterY) / 2,
                   SSD1306_BLACK);
  disp->display();
  delay(hold);
}

// Standard mouth
void Face::drawSmileMouth() {
  int screenCenterX = disp->width() / 2;
  int mouthCenterY = disp->height() - MOUTH_OFFSET - MOUTH_RADIUS;

  // doubled line
  disp->drawCircle(screenCenterX, mouthCenterY, MOUTH_RADIUS, SSD1306_WHITE);
  disp->drawCircle(screenCenterX, mouthCenterY, MOUTH_RADIUS - 1, SSD1306_WHITE);

  // erase top part
  disp->fillRect(0, 0,
                   disp->width(), calcTopOfMouth(),
                   SSD1306_BLACK);
}

void Face::drawLaughingMouth() {
  int screenCenterX = disp->width() / 2;
  int mouthCenterY = disp->height() - MOUTH_OFFSET - MOUTH_RADIUS;

  // doubled line
  disp->drawCircle(screenCenterX, mouthCenterY, MOUTH_RADIUS, SSD1306_WHITE);
  disp->drawCircle(screenCenterX, mouthCenterY, MOUTH_RADIUS - 1, SSD1306_WHITE);

  // erase top part
  disp->fillRect(0, 0,
                   disp->width(), calcTopOfMouth() - MOUTH_HEIGHT,
                   SSD1306_BLACK);
}

void Face::drawOooMouth() {
  int screenCenterX = disp->width() / 2;
  int oooRadius = MOUTH_HEIGHT * 1.5;
  int mouthCenterY = disp->height() - MOUTH_OFFSET - oooRadius;

  disp->drawCircle(screenCenterX, mouthCenterY, oooRadius, SSD1306_WHITE);
  disp->drawCircle(screenCenterX, mouthCenterY, oooRadius - 1, SSD1306_WHITE);
}

/**
   Draw "standard" eyes, shifted left and up pixels in those directions
   (left and up can be negative)
*/
void Face::drawEyes(int left, int up) {
  int leftEyeCenterX = calcLeftEyeCenterX(left, up);
  int rightEyeCenterX = calcRightEyeCenterX(left, up);
  int eyeCenterY = calcEyeCenterY(up);

  // usually we want highlights
  drawEyes(left, up, leftEyeCenterX, rightEyeCenterX, eyeCenterY);
}

/**
   Laugh eyes are wider
*/
void Face::drawLaughEyes(int left, int up, int leftEyeCenterX, int rightEyeCenterX, int eyeCenterY) {
  int squintWidening = 4;

  // draw white
  disp->fillCircle(leftEyeCenterX,
                     eyeCenterY,
                     EYE_RADIUS + squintWidening,
                     SSD1306_WHITE);

  disp->fillCircle(rightEyeCenterX,
                     eyeCenterY,
                     EYE_RADIUS + squintWidening,
                     SSD1306_WHITE);

  // draw pupil
  disp->fillCircle(leftEyeCenterX,
                     eyeCenterY,
                     PUPIL_RADIUS + squintWidening,
                     SSD1306_INVERSE);

  disp->fillCircle(rightEyeCenterX,
                     eyeCenterY,
                     PUPIL_RADIUS + squintWidening,
                     SSD1306_INVERSE);

  int shrinkHighlight = 0;
  int raiseHighlight = 6;

  // draw highlights
  disp->fillCircle(calcLeftEyeHighlightX(left, up),
                     calcEyeHighlightY(up) - raiseHighlight,
                     HILITE_RADIUS - shrinkHighlight,
                     SSD1306_INVERSE);

  disp->fillCircle(calcRightEyeHighlightX(left, up),
                     calcEyeHighlightY(up) - raiseHighlight,
                     HILITE_RADIUS - shrinkHighlight,
                     SSD1306_INVERSE);

                     
  int percentSquint = 60;
  // circle pushing up eye from below
  int cheekCenterY = map(percentSquint, 100, 0,
                         calcCheekCenterSquinting(up, eyeCenterY),
                         calcCheekCenterNoSquinting(up, eyeCenterY));
                         

                      // deform eye
  disp->fillCircle(leftEyeCenterX,
                     cheekCenterY,
                     EYE_RADIUS,
                     SSD1306_WHITE);

  disp->fillCircle(leftEyeCenterX,
                     cheekCenterY,
                     EYE_RADIUS - 2,
                     SSD1306_BLACK);

  disp->fillCircle(rightEyeCenterX,
                     cheekCenterY,
                     EYE_RADIUS,
                     SSD1306_WHITE);

  disp->fillCircle(rightEyeCenterX,
                     cheekCenterY,
                     EYE_RADIUS - 2,
                     SSD1306_BLACK);

  // "erase" bottom parts
  disp->fillRect(leftEyeCenterX - EYE_RADIUS, (eyeCenterY + cheekCenterY) / 2,
                   2 * EYE_RADIUS + 2, 2 * EYE_RADIUS,
                   SSD1306_BLACK);

  disp->fillRect(rightEyeCenterX - EYE_RADIUS, (eyeCenterY + cheekCenterY) / 2,
                   2 * EYE_RADIUS + 2, 2 * EYE_RADIUS,
                   SSD1306_BLACK);
}

/**
   Standard circular eye with highlights
*/
void Face::drawEyes(int left, int up, int leftEyeCenterX, int rightEyeCenterX, int eyeCenterY) {
  // draw white
  disp->fillCircle(leftEyeCenterX,
                     eyeCenterY,
                     EYE_RADIUS,
                     SSD1306_WHITE);

  disp->fillCircle(rightEyeCenterX,
                     eyeCenterY,
                     EYE_RADIUS,
                     SSD1306_WHITE);

  // draw pupil
  disp->fillCircle(leftEyeCenterX,
                     eyeCenterY,
                     PUPIL_RADIUS,
                     SSD1306_INVERSE);

  disp->fillCircle(rightEyeCenterX,
                     eyeCenterY,
                     PUPIL_RADIUS,
                     SSD1306_INVERSE);

  // draw highlights
  disp->fillCircle(calcLeftEyeHighlightX(left, up),
                     calcEyeHighlightY(up),
                     HILITE_RADIUS,
                     SSD1306_INVERSE);

  disp->fillCircle(calcRightEyeHighlightX(left, up),
                     calcEyeHighlightY(up),
                     HILITE_RADIUS,
                     SSD1306_INVERSE);
}

int Face::calcTopOfMouth() {
  return disp->height() - (MOUTH_OFFSET + MOUTH_HEIGHT);
}

int Face::calcLeftEyeCenterX(int left, int up) {
  return EYE_X_INSET + EYE_RADIUS - left;
}

int Face::calcRightEyeCenterX(int left, int up) {
  return disp->width() - EYE_X_INSET - EYE_RADIUS - left - 1;
}

int Face::calcEyeCenterY(int up) {
  return EYE_RADIUS + EYE_Y_INSET - up;
}

int Face::calcLeftEyeHighlightX(int left, int up) {
  int eyeCenter = calcLeftEyeCenterX(left, up);
  return eyeCenter + HILITE_OFFSET_X;
}

int Face::calcRightEyeHighlightX(int left, int up) {
  int eyeCenter = calcRightEyeCenterX(left, up);
  return eyeCenter + HILITE_OFFSET_X;
}

int Face::calcEyeHighlightY(int up) {
  int eyeCenter = calcEyeCenterY(up);
  return eyeCenter - HILITE_OFFSET_Y;
}

int Face::calcLidCenterOpen(int up, int eyeCenterY) {
  return eyeCenterY - 2 * EYE_RADIUS;
}

int Face::calcLidCenterClosed(int up, int eyeCenterY) {
  return eyeCenterY;
}

int Face::calcCheekCenterSquinting(int up, int eyeCenterY) {
  return eyeCenterY;
}

int Face::calcCheekCenterNoSquinting(int up, int eyeCenterY) {
  return eyeCenterY + 2 * EYE_RADIUS;
}

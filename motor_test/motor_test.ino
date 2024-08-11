/**
 * First attempt at getting a motor spinning
 * 
 * Hardware:
 * - brushless DC motor (5V)
 * - ESP32
 * - Adafruit TB6612 1.2A DC/Stepper Motor Driver Breakout Board
 */

int buttonState = 0;

// read a button - if pressed run motor backwards
const int buttonPin = 36;

// motor controller needs 2 inputs, IN1 and IN2
// PWM and STBY are pulled to high
const int Ain1Pin = 4;
const int Ain2Pin = 5;
const int Bin1Pin = 18;
const int Bin2Pin = 19;

void setup() {
 Serial.begin(115200);

  pinMode(buttonPin, INPUT);

  pinMode(Ain1Pin, OUTPUT);
  pinMode(Ain2Pin, OUTPUT);
  pinMode(Bin1Pin, OUTPUT);
  pinMode(Bin2Pin, OUTPUT);
}

void loop() {

  //===============================================
  // Code to change motor direction on button press
  buttonState = digitalRead(buttonPin);
  Serial.println(buttonState);

  if(buttonState == HIGH) {
    digitalWrite(Ain1Pin, HIGH);
    digitalWrite(Ain2Pin, LOW);
    digitalWrite(Bin1Pin, HIGH);
    digitalWrite(Bin2Pin, LOW);
  } else {
    digitalWrite(Ain1Pin, LOW);
    digitalWrite(Ain2Pin, HIGH);
    digitalWrite(Bin1Pin, LOW);
    digitalWrite(Bin2Pin, HIGH);
  }
  //===============================================

  // Uncomment for code to simply run motors forward and backward
//  Serial.println("forward");
//  digitalWrite(Ain1Pin, HIGH);
//  digitalWrite(Ain2Pin, LOW);
//  digitalWrite(Bin1Pin, HIGH);
//  digitalWrite(Bin2Pin, LOW);
//  // I know delay() is funky, it's just for testing
//  delay(2000);
//  
//  Serial.println("backward");
//  digitalWrite(Ain1Pin, LOW);
//  digitalWrite(Ain2Pin, HIGH);
//  digitalWrite(Bin1Pin, LOW);
//  digitalWrite(Bin2Pin, HIGH);
//  delay(2000);

}

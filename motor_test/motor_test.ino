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
//const int buttonPin = 4;

// motor controller needs 2 inputs, IN1 and IN2
// PWM and STBY are pulled to high
const int in1Pin = 4;
const int in2Pin = 5;

void setup() {
 Serial.begin(115200);

  //pinMode(buttonPin, INPUT);

  pinMode(in1Pin, OUTPUT);
  pinMode(in2Pin, OUTPUT);
}

void loop() {
  //buttonState = digitalRead(buttonPin);
 // Serial.println(buttonState);
Serial.println("forward");
    digitalWrite(in1Pin, HIGH);
    digitalWrite(in2Pin, LOW);
  // I know this is funky it's just for testing
  delay(2000);
  Serial.println("backward");
    digitalWrite(in1Pin, LOW);
    digitalWrite(in2Pin, HIGH);
  delay(2000);

//  if(buttonState == HIGH) {
//    digitalWrite(in1Pin, HIGH);
//    digitalWrite(in2Pin, LOW);
//  } else {
//    digitalWrite(in1Pin, LOW);
//    digitalWrite(in2Pin, HIGH);
//  }
}

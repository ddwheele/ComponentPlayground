// Goal: read value from pot, print value, use value to set PWM to Servo

const int potPin = 4;
const int servoPin = 18;

int potValue = 0;
float converted;

void setup() {
  Serial.begin(115200);
  delay(1000);
  pinMode(potPin, INPUT);
  pinMode(servoPin, OUTPUT);
  Serial.println("**********************************");
  Serial.println("**********************************");
  Serial.println("Starting Servo Test:");
}

void loop() {
  // put your main code here, to run repeatedly:
  potValue = analogRead(potPin);
  Serial.println(potValue);
  // need to convert 0-4096 to 0-255
  converted = ((float)potValue)*(0.06227106227);

  Serial.print((int)converted);
  Serial.println(" is (int)converted");

  analogWrite(servoPin, (int)converted);
  delay(500);
}

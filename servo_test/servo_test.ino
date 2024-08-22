// Goal: read value from pot, print value, use value to set PWM to Servo

const int POT_PIN = 4;
const int IN1_PIN = 18;
const int PWM_PIN = 19;

int potValue = 0;
float converted;

void setup() {
  Serial.begin(115200);
  delay(1000);
  pinMode(POT_PIN, INPUT);
  pinMode(IN1_PIN, OUTPUT);
  pinMode(PWM_PIN, OUTPUT);
  Serial.println("**********************************");
  Serial.println("**********************************");
  Serial.println("*** Starting Servo Test:");
}

void loop() {
  // put your main code here, to run repeatedly:
/*  potValue = analogRead(POT_PIN);
  Serial.println(potValue);
  // need to convert 0-4096 to 0-255
  converted = ((float)potValue)*(0.06227106227);

  Serial.print((int)converted);
  Serial.println(" is (int)converted");
   analogWrite(IN1_PIN, (int)converted);
*/
 Serial.println("HIGH");
 
 for(int i=0; i<255; i+=10) {
     digitalWrite(IN1_PIN, HIGH);
     analogWrite(PWM_PIN, i);
     Serial.println(i);
     delay(2000);
  }

  for(int i=254; i>=0; i-=10) {
     digitalWrite(IN1_PIN, HIGH);
     analogWrite(PWM_PIN, i);
     Serial.println(i);
     delay(2000);
  }

}

/**
 * Using a T-Slot Photo Interrupter with a TT Motor Encoder
 * to count "clicks" as wheel turns.
 * 
 * Using Feather
 * 
 * Photo Interrupter leads
 * - (+) 5V
 * - L 5V
 * - OUT - pullup 
 * - (-) Ground
 */

const int ENCODER_IN = 13; // input pin for the interrupter 
int detectState=0; // Variable for reading the encoder status

void setup() {
     pinMode(ENCODER_IN, INPUT_PULLUP); 

}

void loop() {
  detectState=digitalRead(ENCODER_IN);
   if (detectState == HIGH) { //If encoder output is high
    Serial.println(F("HIGH"));
   }
   else {
       Serial.println(F("LOW"));
   }
}

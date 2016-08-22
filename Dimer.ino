int ledPin = 6;

void setup() {}

void loop() {
  for (int fadeValue = 50 ; fadeValue <= 255; fadeValue += 3) {  // sets the value (range from 0 to 255):
    analogWrite(ledPin, fadeValue);
    delay(50);
  }

  for (int fadeValue = 255 ; fadeValue >= 50; fadeValue -= 3) {
    analogWrite(ledPin, fadeValue);
    delay(50);
  }
}

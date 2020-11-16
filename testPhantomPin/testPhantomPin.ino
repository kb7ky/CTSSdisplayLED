const int inputPin = 12;
const int outputPin = 13;

void setup() {
  pinMode(inputPin, INPUT_PULLUP);
  pinMode(outputPin, OUTPUT);
}

void loop() {
  digitalWrite(outputPin, 0);
}

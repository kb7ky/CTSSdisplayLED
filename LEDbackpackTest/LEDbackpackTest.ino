#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_LEDBackpack.h>

// instanciate LED
Adafruit_7segment matrix = Adafruit_7segment();

void setup() {
  matrix.begin(0x70);
}

void loop() {
  for (uint16_t counter = 0; counter < 9999; counter++) {
    matrix.println(counter/10.0);
    matrix.writeDisplay();
    delay(10);
  }
}

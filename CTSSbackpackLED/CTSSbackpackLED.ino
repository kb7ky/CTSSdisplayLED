// sketch for displaying/setting CTSS code on a CommSpec SP32
// adafruit Metro Mini board
// using adafruit 7 segment LED backpack
//
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_LEDBackpack.h>


// Save Last Set Freq for next power up
#include <EEPROM.h>
const int EEPROM_IDX_ADDR = 0;

// Metro Mini I2C Pins
// SDA = A4
// SCLK = A5

// CTSS Pins
const int ctssOnePin = 2;
const int ctssTwoPin = 3;
const int ctssThreePin = 4;
const int ctssFourPin = 5;
const int ctssFivePin = 6;

// instanciate LED backpack matrix
Adafruit_7segment matrix = Adafruit_7segment();

// Button Pins
//Pin connected to SW1
const int buttonOnePin = 9;
//Pin connected to SW2
const int buttonTwoPin = 10;
//Pin connected to SW3
const int buttonThreePin = 11;
//Pin connected to SW4
const int buttonFourPin = 12;

// CTSS Definitions
struct CTSS {
  int freq;
  int value;
};

// Notes:
// 0 = ON = CLOSED = GND
// 1 = OFF = OPEN
// Value Bits are MSB -> LSB - i.e. 5,4,3,2,1
// Freq is 10x and decimal point added during LED display
//
struct CTSS ctss[] = {
  { 670, 0x0 },
  { 719, 0x01 },
  { 744, 0x02 },
  { 770, 0x03 },
  { 797, 0x04 },
  { 825, 0x05 },
  { 854, 0x06 },
  { 885, 0x07 },
  { 915, 0x08 },
  { 948, 0x09 },
  { 974, 0x0a },
  { 1000, 0x0b },
  { 1035, 0x0c },
  { 1072, 0x0d },
  { 1109, 0x0e },
  { 1148, 0x0f },
  { 1188, 0x10 },
  { 1230, 0x11 },
  { 1273, 0x12 },
  { 1318, 0x13 },
  { 1365, 0x14 },
  { 1413, 0x15 },
  { 1462, 0x16 },
  { 1514, 0x17 },
  { 1567, 0x18 },
  { 1622, 0x19 },
  { 1679, 0x1a },
  { 1738, 0x1b },
  { 1799, 0x1c },
  { 1862, 0x1d },
  { 1928, 0x1e },
  { 2035, 0x1f }
};
const int NUMCTSS = 32;
int ctssIdx = 0;
const int NUMCTSSPIN = 5;
const int ctssPins[] = {
  ctssOnePin,
  ctssTwoPin,
  ctssThreePin,
  ctssFourPin,
  ctssFivePin
};

// CTSS_MONITOR sets pins to CTSSPINMODE - used to watch
const int CTSS_MONITOR = 0;
const int CTSSPINMODE = INPUT;

// CTSS_ACTIVE sets pins to OUTPUT - used to drive Commspec CTSS
const int CTSS_ACTIVE = 1;

// Change the mode to CTSS_MONITOR or CTSS_ACTIVE
int ctssMode = CTSS_ACTIVE;

#define NUMBUTTON 4

// debug stuff
int dbglvl = 6;
#define DEBUG(lvl, val)  if(lvl < dbglvl) Serial.println(val);

// Button Stuff
const int BUTTON_READY = 0;
const int BUTTON_PRESS = 1;
const int BUTTON_PROCESS = 2;

int buttonPins[] = {
  buttonOnePin,
  buttonTwoPin,
  buttonThreePin,
  buttonFourPin
};
int buttonState[4];

int currentLEDval = 0;

void setDisplay(int val) {
  if(val != currentLEDval) {
    matrix.println(val/10.0);
    matrix.writeDisplay();
    currentLEDval = val;
    DEBUG(4,"setDisplay ");
    DEBUG(4, val);
  }
}

void nextCTSS() {
  ctssIdx++;
  ctssIdx %= 32;
  EEPROM.update(EEPROM_IDX_ADDR, ctssIdx);
}

void prevCTSS() {
  if(ctssIdx == 0) {
    ctssIdx = NUMCTSS -1;
  } else {
    --ctssIdx;
  }
  EEPROM.update(EEPROM_IDX_ADDR, ctssIdx);
}

void memCTSS1() {
  ctssIdx = 11;
  EEPROM.update(EEPROM_IDX_ADDR, ctssIdx);
}

void memCTSS2() {
  ctssIdx = 25;
  EEPROM.update(EEPROM_IDX_ADDR, ctssIdx);
}

int currentCTSSidx = 0;
void setCTSS(int idx) {
  int val = ctss[idx].value;
  int outval = 0;

  // skip if already set
  if(idx != currentCTSSidx) {
    for(int i = 0; i < NUMCTSSPIN; i++) {
      // shift out LSB -> MSB
      outval = ((val & 0x01));
      digitalWrite(ctssPins[i], outval);
      val = val >> 1;
    }
    currentCTSSidx = idx;
  }
}

int scanCTSS() {
  int val = 0;
  int pinval = 0;

  // read MSB to LSB
  for(int i=NUMCTSSPIN - 1; i >=  0; --i) {
    pinval = digitalRead(ctssPins[i]);
    val |= ((pinval) << i);
  }

  if(val != ctssIdx) {
    DEBUG(4,"scanVal ");
    DEBUG(4, val);
  }
  return val;
}

bool scanButtons() {
  int swIn = 0;
  for(int i = 0; i < NUMBUTTON; i++) {
    swIn = digitalRead(buttonPins[i]);
    if((swIn == 0) && (buttonState[i] == BUTTON_READY)) {
      buttonState[i] = BUTTON_PRESS;
      DEBUG(5,"Switch ");
      DEBUG(5,i);
      DEBUG(5," Pressed");
    }

    if((swIn == 1) && (buttonState[i] == 1)) {
      buttonState[i] = BUTTON_PROCESS;
      DEBUG(5,"Switch ");
      DEBUG(5,i);
      DEBUG(5," Released");  
    }
  }
}

void processButtons() {
  for(int i = 0; i < NUMBUTTON; i++) {
    if(buttonState[i] == BUTTON_PROCESS) {
      buttonState[i] = BUTTON_READY;
      switch(i) {
        case 0:
          // process Button1
          nextCTSS();
          break;
        case 1:
          // process Button2
          prevCTSS();
          break;
        case 2:
          // process Button3
          memCTSS1();
          break;
        case 3:
          // process Button 4
          memCTSS2();
          break;
        default:
          DEBUG(1,"Error in processButtons case");
          break;  
      }
    }
  }
}

void setup() {
  int idx = 0;
  
  // debug
  Serial.begin(57600);
  DEBUG(5,"Startup Serial");

  // setup LED backpack
  matrix.begin(0x70);
  
  if(ctssMode == CTSS_MONITOR) {
    for(int i = 0; i < NUMCTSSPIN; i++) {
        pinMode(ctssPins[i], CTSSPINMODE);
    }
    DEBUG(4,"Mode = MONITOR");
  } else {
    for(int i = 0; i < NUMCTSSPIN; i++) {
      pinMode(ctssPins[i], OUTPUT);
    }
    ctssMode = CTSS_ACTIVE;
    DEBUG(4,"Mode = ACTIVE");
    ctssIdx = EEPROM.read(EEPROM_IDX_ADDR);
    if(ctssIdx < 0 || ctssIdx > 31) {
      ctssIdx = 0;
      EEPROM.update(EEPROM_IDX_ADDR, ctssIdx);
    }
  }
  
  // setup buttonPins
  for(int i = 0; i < NUMBUTTON; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
    buttonState[i] = BUTTON_READY;
  }
}

void loop() {
  if(ctssMode == CTSS_MONITOR) {
    ctssIdx = scanCTSS();
  } else {
    setCTSS(ctssIdx);
  }
  
  scanButtons();
  processButtons();

  setDisplay(ctss[ctssIdx].freq);
  
}

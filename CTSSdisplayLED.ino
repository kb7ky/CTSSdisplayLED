// sketch for displaying/setting CTSS code on a CommSpec SP32
// also from sketch to drive 7 segment LED's on Modtronics Dev7 Board
//

// Save Last Set Freq for next power up
#include <EEPROM.h>
const int EEPROM_IDX_ADDR = 0;

// CTSS Pins
const int ctssOnePin = A0;
const int ctssTwoPin = A1;
const int ctssThreePin = A2;
const int ctssFourPin = A3;
const int ctssFivePin = A4;

// LED Pins
//Pin connected to ST_CP of 74HC595
const int latchPin = 8;
//Pin connected to SH_CP of 74HC595
const int clockPin = 12;
//Pin connected to DS of 74HC595
const int dataPin = 11;
//Pin connected to I1 of ULN2003
const int sevenOnePin = 4;
//Pin connected to I2 of ULN2003
const int sevenTwoPin = 5;
//Pin connected to I3 of ULN2003
const int sevenThreePin = 6;
//Pin connected to I4 of ULN2003
const int sevenFourPin = 7;

// Button Pins
//Pin connected to SW1
const int buttonOnePin = 9;
//Pin connected to SW2
const int buttonTwoPin = 10;
//Pin connected to SW3
const int buttonThreePin = 13;
//Pin connected to SW4
const int buttonFourPin = 3;

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

// Seven Segment bar definitions
#define SEGA 0x01
#define SEGB 0x02
#define SEGC 0x04
#define SEGD 0x08
#define SEGE 0x10
#define SEGF 0x20
#define SEGG 0x40
#define SEGDP 0x80

// dev7 board definitions
#define NUMDIGIT 4
#define NUMBUTTON 4

// debug stuff
int dbglvl = 5;
#define DEBUG(lvl, val)  if(lvl < dbglvl) Serial.println(val);

// array mappping numbers to values
const byte segmap[] = {
  SEGA | SEGB | SEGC | SEGD | SEGE | SEGF,
  SEGB | SEGC,
  SEGA | SEGB | SEGG | SEGE | SEGD,
  SEGA | SEGB | SEGG | SEGC | SEGD,
  SEGF | SEGB | SEGG | SEGC,
  SEGA | SEGF | SEGG | SEGC | SEGD,
  SEGF | SEGE | SEGG | SEGC | SEGD | SEGA,
  SEGA | SEGB | SEGC,
  SEGA | SEGB | SEGC | SEGD | SEGE | SEGF | SEGG,
  SEGA | SEGF | SEGB | SEGG | SEGC | SEGD
};

const int digitPins[] = {
  sevenOnePin,
  sevenTwoPin,
  sevenThreePin,
  sevenFourPin
};

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

static const long powersOf10[] = {
  1, // 10^0
  10,
  100,
  1000,
  10000,
  100000,
  1000000,
  10000000,
  100000000,
  1000000000
}; // 10^9

int prevUpdateIdx = 0;
unsigned long prevUpdateTime = 0;
int ledOnTime = 2000; // Corresponds to a brightness of 100

byte digit[] = {
  0,0,0,0
};

void setDisplay(long val) {
  // Find all digits for base's representation, starting with the most significant digit
  for (int digitNum = 0; digitNum < NUMDIGIT ; digitNum++) {
    long factor = powersOf10[NUMDIGIT - 1 - digitNum];
    digit[digitNum] = val / factor;
    val -= digit[digitNum] * factor;
  }
}

void turnOffAll() {
  for (int i = 0; i < NUMDIGIT; i++) {
    digitalWrite(digitPins[i], LOW);
  }
}

void digitOff(int idx) {
  digitalWrite(digitPins[idx], LOW);
}

void incrDigit(int idx) {
  if(digit[idx] >= 9) {
    digit[idx] = 0;
  } else {
    digit[idx] += 1;
  }
}

void digitOn(int idx) {
   int segments = segmap[digit[idx]];
   DEBUG(5,"digitOn");
   DEBUG(5,idx);
   DEBUG(5,digit[idx]);

   if((idx != 0) && ((idx % 2) == 0)) {
    segments |= SEGDP;
   }
   
   digitalWrite(latchPin, LOW);
   shiftOut(dataPin, clockPin, MSBFIRST, segments);
   digitalWrite(latchPin, HIGH);  
   digitalWrite(digitPins[idx], HIGH); 
}

void refreshAll() {
  unsigned long us = micros();
  if (us - prevUpdateTime < ledOnTime) return;
  prevUpdateTime = us;

  // turn off current digit
  digitOff(prevUpdateIdx);

  // setup for next digit (with wrap)
  prevUpdateIdx++;
  if (prevUpdateIdx >= NUMDIGIT) prevUpdateIdx = 0;

  // Illuminate the required segments for the new digit
  digitOn(prevUpdateIdx);
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
    
  //set pins to output because they are addressed in the main loop
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  // setup digitPins - hooked to cathode of LEDs
  for (int i = 0; i < NUMDIGIT; i++) {
    pinMode(digitPins[i], OUTPUT);
  }
  
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
  }
  
  // setup buttonPins
  for(int i = 0; i < NUMBUTTON; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
    buttonState[i] = BUTTON_READY;
  }
  
  // setup timer
  prevUpdateTime = micros();
  
  // All digits OFF
  turnOffAll();
}

void loop() {
  refreshAll();

  if(ctssMode == CTSS_MONITOR) {
    ctssIdx = scanCTSS();
  } else {
    setCTSS(ctssIdx);
  }
  
  scanButtons();
  processButtons();

  setDisplay(ctss[ctssIdx].freq);
  
}

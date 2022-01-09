

#include <Arduino.h>
#include <U8g2lib.h>
#include <SPI.h>
#include <Wire.h>

// Rotary Encoder Inputs
#define CLK 2
#define DT 3
#define rotaryBtnPin 4

int weldDelay = 50;
// # of welds since last reboot
int weldCount = 0;
boolean showWeldCount = false;
int currentStateCLK;
int lastStateCLK;
String currentDir = "";


int oledPin_SCL = A5;
int oledPin_SDA = A4;
// https://github.com/olikraus/u8g2/wiki/u8g2setupcpp#wiring
U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, oledPin_SCL, oledPin_SDA);

// Button Pin
int weldBtnPin = 7;
// SSR Pin
int ssrPin = 10;
int buttonState;

int rotaryButtonState;
int lastButtonState = LOW;
int lastRotaryButtonState = LOW;
int stringWidth = 0;


unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long lastRotaryDebounceTime = 0;
unsigned long lastWeldDebounceTime = 0;
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers
int weldDebounceDelay = 50; // MS to initialize the weld time to
int maxWeldDelay = 500;
int minWeldDelay = 10;

void setup(void) {
  pinMode(ssrPin, OUTPUT);
  // Make sure the SSR Pin starts LOW
  digitalWrite(ssrPin, LOW);
  u8g2.begin();
  // Control
  // Set encoder pins as inputs
  pinMode(CLK, INPUT);
  pinMode(DT, INPUT);
  pinMode(rotaryBtnPin, INPUT);
  // Read the initial state of CLK
  lastStateCLK = digitalRead(CLK);
  // Pin 0 and Pin 1 (interrupt pins)
  attachInterrupt(0, readEncoder, CHANGE);
  attachInterrupt(1, readEncoder, CHANGE);

  // Welding
  pinMode(weldBtnPin, INPUT_PULLUP);

}


void draw(void) {
  // Show the weld delay
  u8g2.setFont(u8g2_font_ncenB12_tr);
  if (showWeldCount) {
    u8g2.drawStr(1, 13, "Weld Count");
    u8g2.drawStr(1, 60, String(weldCount).c_str());
    u8g2.drawStr( 55, 60, "welds");
  } else {
    u8g2.drawStr(10, 13, "Weld Delay");
    u8g2.setFont(u8g2_font_ncenB24_tr);
    if (weldDebounceDelay > 99)
      u8g2.drawStr(1, 60, String(weldDebounceDelay).c_str());
    else
      u8g2.drawStr(25, 60, String(weldDebounceDelay).c_str());
    u8g2.drawStr( 55, 60, " ms");
  }
}

void loop(void) {
  // Draw Menu
  u8g2.firstPage();
  do {
    draw();
  } while ( u8g2.nextPage() );
  /* =========================================== */
  /*      Handle Welder                          */
  /* =========================================== */
  int reading = digitalRead(weldBtnPin);
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }
  /*=================*/
  /* WELDER START    */
  /*=================*/
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;
      if (buttonState == HIGH) {
        digitalWrite(ssrPin, HIGH);
        weldCount++;
        lastWeldDebounceTime = millis();
      }
    }
  }
  lastButtonState = reading;
  /*==================*/
  /* WELD STOP        */
  /*==================*/

  // Wait until delay has passed then turn off weld
  if ((millis() - lastWeldDebounceTime) > weldDebounceDelay) {
    digitalWrite(ssrPin, LOW);
  }

  /* =========================================== */
  /*      END Welder Handler                     */
  /* =========================================== */

  /* =========================================== */
  /*      MENU BTN PRESS                         */
  /* =========================================== */
  int rotary_reading = digitalRead(rotaryBtnPin);
  if (rotary_reading != lastRotaryButtonState) {
    lastRotaryDebounceTime = millis();
  }
  if ((millis() - lastRotaryDebounceTime) > debounceDelay) {
    if (rotary_reading != rotaryButtonState) {
      rotaryButtonState = rotary_reading;
      if (rotaryButtonState == LOW) {
        // Do something in the OLED menu
        showWeldCount = !showWeldCount;
      }
    }
  }
  lastRotaryButtonState = rotary_reading;

}


void readEncoder() {
  currentStateCLK = digitalRead(CLK);
  if (currentStateCLK != lastStateCLK  && currentStateCLK == 1) {
    
    if (digitalRead(DT) == currentStateCLK) {
      if (weldDelay > minWeldDelay) {
        weldDelay -= 10;
        currentDir = "CCW";
      }
    } else {
      if (weldDelay < maxWeldDelay) {
        weldDelay += 10;
        currentDir = "CW";
      }
    }
    weldDebounceDelay = weldDelay;
  }
  lastStateCLK = currentStateCLK;
}

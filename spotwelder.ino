

#include <Arduino.h>
#include <U8g2lib.h>
#include <SPI.h>
#include <Wire.h>

// Rotary Encoder Inputs
#define CLK 2
#define DT 3
#define rotaryBtnPin 4

int counter = 50;
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
  u8g2.begin();
  // Control
  // Set encoder pins as inputs
  pinMode(CLK, INPUT);
  pinMode(DT, INPUT);
  pinMode(rotaryBtnPin, INPUT);
  // Read the initial state of CLK
  lastStateCLK = digitalRead(CLK);
  // Call updateEncoder() when any high/low changed seen
  // on interrupt 0 (pin 2), or interrupt 1 (pin 3)
  attachInterrupt(0, updateEncoder, CHANGE);
  attachInterrupt(1, updateEncoder, CHANGE);

  // Welding
  pinMode(weldBtnPin, INPUT_PULLUP);
  pinMode(ssrPin, OUTPUT);

  // Test
  digitalWrite(ssrPin, HIGH);
  delay(2000);

  Serial.begin(9600);
}


void draw(void) {
  // graphic commands to redraw the complete screen
  u8g2.setFont(u8g2_font_ncenB12_tr);
  u8g2.drawStr(10, 13, "Weld Delay");
  // u8g2.setScale2x2();
  //  u8g2.setPrintPos(20,20);
  u8g2.setFont(u8g2_font_ncenB24_tr);
  if (weldDebounceDelay > 99)
    u8g2.drawStr(1, 60, String(weldDebounceDelay).c_str());
  else
    u8g2.drawStr(25, 60, String(weldDebounceDelay).c_str());
  // u8g2.undoScale();
  u8g2.drawStr( 55, 60, " ms");

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
  // This is for the initial button press
  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current s tate:
    // if the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;
      if (buttonState == HIGH) {
        digitalWrite(ssrPin, HIGH);
        lastWeldDebounceTime = millis();
       // delay(weldDebounceDelay);
       // digitalWrite(ssrPin, LOW);
        Serial.println("SSR Weld Button Pressed!");
        
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
  /*      MENU HANDLER                           */
  /* =========================================== */
  int rotary_reading = digitalRead(rotaryBtnPin);
  if (rotary_reading != lastRotaryButtonState) {
    // reset the debouncing timer
    lastRotaryDebounceTime = millis();
  }
  if ((millis() - lastRotaryDebounceTime) > debounceDelay) {
    if (rotary_reading != rotaryButtonState) {
      rotaryButtonState = rotary_reading;
      if (rotaryButtonState == LOW) {
        Serial.println("Rotary Button Pressed!");
      }
    }
  }
  lastRotaryButtonState = rotary_reading;

}


void updateEncoder() {
  // Read the current state of CLK
  currentStateCLK = digitalRead(CLK);
  // If last and current state of CLK are different, then pulse occurred
  // React to only 1 state change to avoid double count
  if (currentStateCLK != lastStateCLK  && currentStateCLK == 1) {
    Serial.println(counter);
    if (digitalRead(DT) == currentStateCLK) {
      if (counter > minWeldDelay) {
        counter -= 10;
        currentDir = "CCW";
      }
    } else {
      if (counter < maxWeldDelay) {
        counter += 10;
        currentDir = "CW";
      }
    }
    weldDebounceDelay = counter;

    Serial.print("Direction: ");
    Serial.print(currentDir);
    Serial.print(" | Counter: ");
    Serial.println(counter);
  }

  // Remember last CLK state
  lastStateCLK = currentStateCLK;


}

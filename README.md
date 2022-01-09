# Arduino Spot Welder

### Solid State Relay and Microwave transformer rewound from 1000V+ to 4VAC to spot weld 18650 lithium ion batteries

## Dependencies
* U8g2lib.h - Install Instructions [here](https://github.com/olikraus/u8g2/wiki/u8g2install)
* SPI.h
* Wire.h
* Arduino.h

Uses a rotary decoder to increase/decrease the on time for the spot welder. Includes a status LED connected in parallel with the SSR output pin to know when the welder is live. OLED is used for the display. The library u8g2lib is used for writing the graphics to the OLED.

## Parts List

- OLED [Amazon](https://www.amazon.com/gp/product/B00O2LLT30)
- Spot Welder Pen [Amazon](https://www.amazon.com/gp/product/B08R6Y9DNM)
- Jumper Cables (for wire) [Home Depot](https://www.homedepot.com/p/Road-Power-12-ft-8-Gauge-200-Amp-Yellow-Booster-Cables-84458802/309569068?)
  * You'll want to keep this length very short. Just enough for 3-4 wraps around the transformer. Then about 8 inches on each side from the transformer. If you can get 2/0 - 4/0 gage wire go for it. With the jumper cables you will save money but will need to watch thew cable temperature.
- Rotary Encoder [Amazon](https://www.amazon.com/gp/product/B06XQTHDRR)
- Heavy Duty Wire Lugs [Amazon](https://www.amazon.com/gp/product/B073Y9RYFM)
- Solid State Relay [Amazon](https://www.amazon.com/gp/product/B08GPB7N2T)
- 1/2 inch Heat Shrink [Amazon](https://www.amazon.com/gp/product/B07FPDBGYH/)
- Arduino Pro Mini (Atmega328P) [Amazon](https://www.amazon.com/HiLetgo-Atmega328P-Replace-ATmega128-Atmega328/dp/B07X2JGS69)
  * You could probably use any of the arduino's. If it does not have two interrupts you can change the rotary encoder code or switch to a potentiometer and poll an analog pin instead.
- Various lengths of 12-14 gauge wire for making the connections (Use the jumper cable wire from the transformer to the spot welder pen)
- An old 5v USB power adapter - take it apart and use it as a switch mode power supply for the arduino so you can jump off the mains voltage comming in
- Use the old 120v plug from the microwave
- Build/buy a chassis for this. 
  * I used the metal from the microwave. Grinded/welded it to make a case. (If you use a metal case be sure to ground it with the green/earth/ground line from the 3 prong microwave plug)
  
### IMPORTANT ###
Test all of your code with a bread board. Connect the SSR and everything as if it were "live" but do not connect any high voltage (120v) connections yet. Test everything and make sure it is working as intended before you connect the high voltage lines/transformer. 


*NOTICE* I do not take any responsibility for loss of life / property / or otherwise. Please do your own research. High Voltage is dangerous and can kill. If you are uncertain I suggest using a car battery with high cold cranking amps in leu of a microwave transformer. You will get the current you need at a much safer voltage. 

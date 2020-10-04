# ESP8266 Breakout!

This is a project that I undertook in my first year of University. It's a basic version of the classic atari game 'breakout' made using a wemos esp-8266 board, variable resistor, LED&KEY board and a tiny oled display. 


# Files

The file structure is very basic and should be quite self explanatory. The only required files from the repo are:

 - Ball.cpp
 - Brick.cpp
 - breakout.ino
 
 The program can then be compiled and flashed using the Arduino IDE.

# Wiring
I'll be the first to admit - not the most beautiful thing in the world, but hopefully should provide some assistance if you are hoping to recreate this project or something similar.

![circuit](https://github.com/chrisdinozzi/esp8266-breakout/blob/master/media/wemose.png "Circuit")

# Features
The paddle is controlled using the variable resistor. The gameplay is displayed on the tiny screen and the players 3 lives are tracked using the LEDs on the LED&KEY board, along with the score. 
When the game ends, the using can choose to play again by turning the resistor to the desired option and pressing the first button on the LED&KEY, or simply resetting the wemos board.

# Libraries 

 - Adafruit_GFX.h 
 - Adafruit_SSD1306.h 
 - Streaming.h 
 - iomanip 
 - SPI.h 
 - Wire.h

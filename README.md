# PCD8544 (Nokia 5110) LCD library for usage with the ESP8266

This is a simple implementation made mostly for learning purposes. As of now, it only supports software SPI
connection using five pins and is very limited in its functionality: it doesn't support partial updates, has only one
small font and doesn't inherit all of the functionality of Adafruit's GFX library.

If you're looking for something to use in your projects, I recommend you take a look at
[this first](http://x10linux.blogspot.pt/2015/09/esp8266-and-pcd8544nokia-5110-lcd.html). [github.com/bbx](https://github.com/bbx10/Adafruit-PCD8544-Nokia-5110-LCD-library) forked Adafruit's well known lcd library to add support for the esp8266.

Also take a look at this simple implementation for the Arduino: https://github.com/daniel1111/Nokia5110

I've tested this using a NodeMCU 1.0 with the following connections:

| LCD pin | NodeMCU pin label |
|---------|-------------------|
| RST | D4 (GPIO2) |
| CE | D6 (GPIO12) |
| DC | D5 (GPIO14) |
| Din | D2 (GPIO4) |
| CLK | D1 (GPIO5) |

It is possible to support the lcd with less pins.

## DONE

- pcd8544 initialization routine with parameters
- internal buffer and update routine
- display bitmaps
- drawing functions
- display text with a 4x3 font
- arduino library package structure
- irc connection example (serial i/o connected to tcp socket i/o,
doesn't even respond to ping. you can send commands via serial, and server messages will be dumbly displayed on the lcd screen without any processing)

## PICS

![irc example](http://i.imgur.com/3TkgaOv.jpg)

![connection](http://i.imgur.com/yz6EyXy.jpg)

## TODO
- add support for other connection modes (implicit CS, hardware SPI)
- add more fonts
- improve examples

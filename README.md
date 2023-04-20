# 2023-Pneumatic-Separation
This is the code for detecting apogee and triggering pneumatic separation. Apogee detection is the same as 2023-Flight-Software.

## Board Manager:
Arduino MBED

## Default Pins:
Edit C:\Users\[user]\AppData\Local\Arduino15\packages\arduino\hardware\mbed_rp2040\4.0.2\variants\RASPBERRY_PI_PICO\pins_arduino.h
Lines 56 and 57 should be:

#define PIN_WIRE_SDA (2u)

#define PIN_WIRE_SCL (3u)

## Libraries:
Download the RP2040_PWM library from *this repository* and place in your Arduino/libraries folder. I made changes to this library to make it compatible with the board manager, so it is important to download the updated copy.
The MS5611 library can be downloaded from my repository or this origin repository: https://github.com/jarzebski/Arduino-MS5611

## Pins:

#define SEPARATION 19

#define LED 25

#define PIN_WIRE_SDA (2u)

#define PIN_WIRE_SCL (3u)

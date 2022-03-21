// ------------------------------------------------------------------------
// TTGO-T1 pinout
// -----------------------------------------
// Pinout:
// GND                3.3V
// GND                36.SVP
// 21.WIRE_SDA        37
// >22.WIRE_SCL,      38
// 17.?               39.SVN
//  2. GPIO2          32
// 15. GPIO15         33
// 13. GPIO13         25
// 12. GPIO12         26
// GND                27
// GND                GND
// 3.3V               5V
// 
// LFT_BUT: GPIO0     RGT_BUT: GPIO35
//
// Notes:
//  -LFT_BUT: set INPUT_PULLUP
//  -RGT_BUT: set INPUT
//  -Input-only: 36, 37, 38, 39
//  -ADCs: 2, 15, 13, 12, 36, 39, 32, 33, 25, 26, 27
//  -DACs: 25, 26
//  -I2C: pin 22 is wrong, it should be pin 23 - but no connection
//   => 22 unconnected pin
// ------------------------------------------------------------------------

#pragma once
#include <Arduino.h>

// ------------------------------------------------------------------------
// Buttons setup for TTGO-T1
// ..................................................
#include "Button2.h"
// ..................................................
// Data types
typedef std::function<void(Button2 &btn)> ButtonCallbackFunction;
// ..................................................
// PINS
const int LFT_BUTTON = 0;
const int RGT_BUTTON = 35;
// ..................................................
// Variables
extern Button2 lft_button, rgt_button;
// ..................................................
// Functions
void buttons_init(ButtonCallbackFunction handler);
void loop_buttons(void);
// ..................................................

// ------------------------------------------------------------------------
// I2C
// ..................................................
#include <Wire.h>
// ..................................................
// Pins
const uint8_t I2C_SDA = 21;
const uint8_t I2C_SCL = 15; // 22; -> wrong pinout diagram
// ..................................................
// Functions
void i2c_init(void);

// ------------------------------------------------------------------------
// UARTs
// ..................................................
// UART0 (micro usb)
void uart0_init(const char* msg=NULL);
// ..................................................

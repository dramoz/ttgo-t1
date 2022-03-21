// ------------------------------------------------------------------------
// Setup TTGO-T1 TFT LCD
// TFT_eSPI: LCD control
// -> https://learn.adafruit.com/adafruit-gfx-graphics-library
// --------------------------------------------------------------------------------
#pragma once
// --------------------------------------------------------------------------------
#include <Arduino.h>
#include "TFT_eSPI.h"
// --------------------------------------------------------------------------------
extern TFT_eSPI tft;
// --------------------------------------------------------------------------------
// PRINT Variadic Macros
// TBD

// --------------------------------------------------------------------------------
#ifndef TFT_WIDTH
#define TFT_WIDTH 135
#endif

#ifndef TFT_HEIGHT
#define TFT_HEIGHT 240
#endif
// --------------------------------------------------------------------------------
const uint16_t tft_colors[] = {
    TFT_BLACK      ,
    TFT_NAVY       ,
    TFT_DARKGREEN  ,
    TFT_DARKCYAN   ,
    TFT_MAROON     ,
    TFT_PURPLE     ,
    TFT_OLIVE      ,
    TFT_LIGHTGREY  ,
    TFT_DARKGREY   ,
    TFT_BLUE       ,
    TFT_GREEN      ,
    TFT_CYAN       ,
    TFT_RED        ,
    TFT_MAGENTA    ,
    TFT_YELLOW     ,
    TFT_WHITE      ,
    TFT_ORANGE     ,
    TFT_GREENYELLOW,
    TFT_PINK       ,
    TFT_BROWN      ,
    TFT_GOLD       ,
    TFT_SILVER     ,
    TFT_SKYBLUE    ,
    TFT_VIOLET     
};
const uint8_t tft_colors_size = sizeof(tft_colors);
// --------------------------------------------------------------------------------
// Fonts
// #define LOAD_GLCD   // Font 1. Original Adafruit 8 pixel font needs ~1820 bytes in FLASH
// #define LOAD_FONT2  // Font 2. Small 16 pixel high font, needs ~3534 bytes in FLASH, 96 characters
// #define LOAD_FONT4  // Font 4. Medium 26 pixel high font, needs ~5848 bytes in FLASH, 96 characters
// #define LOAD_FONT6  // Font 6. Large 48 pixel font, needs ~2666 bytes in FLASH, only characters 1234567890:-.apm
// #define LOAD_FONT7  // Font 7. 7 segment 48 pixel font, needs ~2438 bytes in FLASH, only characters 1234567890:-.
// #define LOAD_FONT8  // Font 8. Large 75 pixel font needs ~3256 bytes in FLASH, only characters 1234567890:-.
// #define LOAD_GFXFF  // FreeFonts. Include access to the 48 Adafruit_GFX free fonts FF1 to FF48 and custom fonts
const uint8_t tft_fonts[] = {1, 2, 4, 6, 7, 8};
const uint8_t tft_fonts_size = sizeof(tft_fonts);
extern const char* tft_font_info[];
// --------------------------------------------------------------------------------
extern uint8_t TFT_max_x, TFT_max_y;
void tft_init(bool landscape=false, uint8_t dflt_font=2);
// --------------------------------------------------------------------------------

inline int16_t tft_max_lines(void) { return TFT_max_y / tft.fontHeight(); }

void tft_goto_ln(int ln);
void tft_del_ln(int ln);
void tft_draw_lnxln(int ln, int w=2, int color=TFT_DARKGREEN);
// --------------------------------------------------------------------------------

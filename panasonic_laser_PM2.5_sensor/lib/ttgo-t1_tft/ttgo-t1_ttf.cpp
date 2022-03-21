// --------------------------------------------------------------------------------
#include "ttgo-t1_ttf.h"
// --------------------------------------------------------------------------------
TFT_eSPI tft = TFT_eSPI();
// --------------------------------------------------------------------------------
uint8_t TFT_max_x, TFT_max_y;
const char* tft_font_info[] =
{
    (char*) "8x8p",
    (char*) "16p, ch:96",
    (char*) "26p, ch:96",
    (char*) "48p, ch:nums",
    (char*) "48p, ch:nums",
    (char*) "75p, ch:nums"
};
// --------------------------------------------------------------------------------
void tft_init(bool landscape, uint8_t dflt_font) {
  tft.init();
  if(landscape) {
      tft.setRotation(1);
      TFT_max_x = TFT_HEIGHT;
      TFT_max_y = TFT_WIDTH;
  }
  else {
      tft.setRotation(0);
      TFT_max_x = TFT_WIDTH;
      TFT_max_y = TFT_HEIGHT;
  }
  tft.setCursor(0, 0, dflt_font);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(1);
  
  tft.fillScreen(TFT_BLACK);
}
// --------------------------------------------------------------------------------
void tft_goto_ln(int ln) {
  int16_t h = tft.fontHeight() + 1;
  int16_t max_lines = (TFT_max_y) / h;
  
  if(ln > max_lines)
    return;
  tft.setCursor(0, ln*h);
}
// --------------------------------------------------------------------------------
void tft_draw_lnxln(int ln, int w, int color) {
  int16_t h = tft.fontHeight() + 1;
  int16_t max_lines = (TFT_max_y) / h;
  
  if(ln > max_lines)
    return;
  tft.fillRect(0, ln*h+h/2-w/2, TFT_max_x, w, color);
  tft_goto_ln(ln+1);
}
// --------------------------------------------------------------------------------
void tft_del_ln(int ln) {
  int16_t h = tft.fontHeight() + 1;
  int16_t max_lines = (TFT_max_y) / h;
  
  if(ln > max_lines)
    return;
  
  tft.fillRect(0, ln*h, TFT_max_x, h, tft.textbgcolor);
  tft.setCursor(0, ln*h);
}
// --------------------------------------------------------------------------------

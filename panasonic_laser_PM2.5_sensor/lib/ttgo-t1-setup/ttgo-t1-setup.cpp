// ------------------------------------------------------------------------
#include "ttgo-t1-setup.h"
// ------------------------------------------------------------------------


// ------------------------------------------------------------------------
// Buttons
Button2 lft_button, rgt_button;

void set_button(Button2 &button, int pin, ButtonCallbackFunction handler, byte buttonMode, bool activeLow) {
  button.begin(pin, buttonMode, false, activeLow);
  button.setClickHandler(handler);
  button.setDoubleClickHandler(handler);
  button.setTripleClickHandler(handler);
  button.setLongClickHandler(handler);
}

void buttons_init(ButtonCallbackFunction handler){
  set_button(lft_button, LFT_BUTTON, handler, INPUT_PULLUP, true);
  set_button(rgt_button, RGT_BUTTON, handler, INPUT, true);
}

void buttons_init(ButtonCallbackFunction lft_btn_handler, ButtonCallbackFunction rgt_btn_handler){
  set_button(lft_button, LFT_BUTTON, lft_btn_handler, INPUT_PULLUP, true);
  set_button(rgt_button, RGT_BUTTON, rgt_btn_handler, INPUT, true);
}
void loop_buttons(void){
  lft_button.loop();
  rgt_button.loop();
}

// ------------------------------------------------------------------------
// I2C
void i2c_init(void) {
  Wire.begin (I2C_SDA, I2C_SCL);
  pinMode (I2C_SDA, INPUT_PULLUP);
  pinMode (I2C_SCL, INPUT_PULLUP);
}

// ------------------------------------------------------------------------
// UARTs
void uart0_init(const char *msg) {
  Serial.begin(115200);
  if(msg != nullptr) {
    Serial.println(msg);
  }
}
// ------------------------------------------------------------------------


// ------------------------------------------------------------------------

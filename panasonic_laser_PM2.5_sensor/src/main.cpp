// ------------------------------------------------------------------------
// TTGO-T1
// ------------------------------------------------------------------------
#include <Arduino.h>
#include "Button2.h"
#include "ttgo-t1-setup.h"
// ------------------------------------------------------------------------
#include "gui.h"
clGUI GUI(1000, static_cast<const char*>("Panasonic Laser PM2.5 Sensor"));

// --------------------------------------
#include "panasonic_SN-GCJA5.h"
const uint8_t UART1_RX = 27;
const uint8_t UART1_TX = 26;
clSN_GCJA5 SN_GCJA5(1);

// ------------------------------------------------------------------------
void setup(void) {
  buttons_init(buttons_hndlr);
  GUI.init();
  if(!SN_GCJA5.init(UART1_RX, UART1_TX)) {
    GUI.set(ScreenPage::ERROR);
  }
}
// ------------------------------------------------------------------------
void loop() {
  loop_buttons();
  if(!SN_GCJA5.loop()) {
    GUI.set(ScreenPage::ERROR);
    if(SN_GCJA5.in_sync==false) {
      SN_GCJA5.sync();
    }
  }
  else {
    GUI.loop();
  }
}

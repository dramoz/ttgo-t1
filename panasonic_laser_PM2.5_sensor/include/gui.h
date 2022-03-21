// ------------------------------------------------------------------------
#pragma once
// ------------------------------------------------------------------------
#include <Arduino.h>
#include "ttgo-t1-setup.h"
#include "ttgo-t1_ttf.h"
// ------------------------------------------------------------------------
#include "panasonic_SN-GCJA5.h"
// ------------------------------------------------------------------------
void buttons_hndlr(Button2& btn);
// ------------------------------------------------------------------------
enum class ScreenPage {WLCM, PM, PART_CNT, ERROR};
// --------------------------------------------------------------------------------
class clGUI {
  public:
    clGUI(long refresh_rate, const char wlcm_msg[]);
    void init(void);
    void loop(void);
    void set(ScreenPage screen, bool force=false);
    void draw(void);
    
    void home(void);
    void next_pg(void);
    void prev_pg(void);
    void next_sub_pg(void);
    void prev_sub_pg(void);
    void main_pg(void);
    
    void next_dsply_vl(void);
    void prev_dsply_vl(void);
    
  private:
    long _last_update;
    const char* _wlcm_msg;
    long _refresh_rate;
    ScreenPage _curr;
    uint8_t _subpg_inx;
    uint8_t _dsply_vl;
    void report_reg_vl(uint8_t inx, bool adjst=false);
    void report_reg(uint8_t inx);
    
    friend void buttons_hndlr(Button2& btn);
};

extern clGUI GUI;

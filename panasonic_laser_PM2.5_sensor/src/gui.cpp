// ------------------------------------------------------------------------
// Panasonic Sensor Draw screens
// ------------------------------------------------------------------------
#include "gui.h"
// ------------------------------------------------------------------------
clGUI::clGUI(long refresh_rate, const char* wlcm_msg)
: _last_update(0)
, _wlcm_msg(wlcm_msg)
, _refresh_rate(refresh_rate)
, _curr(ScreenPage::WLCM)
, _subpg_inx(0)
, _dsply_vl(0)
{}
// ------------------------------------------------------------------------
void clGUI::init(void) {
  _last_update = millis();
  
  tft_init(true, 2);  // FONT2: 5 lines
  set(ScreenPage::WLCM, true);
}

// ------------------------------------------------------------------------
void clGUI::loop(void) {
  auto curr_millis = millis();
  if(curr_millis - _last_update > _refresh_rate) {
    _last_update = curr_millis;
    draw();
  }
}

// ------------------------------------------------------------------------
void clGUI::set(ScreenPage new_screen, bool force) {
  if(new_screen!=_curr || force) {
    _curr = new_screen;
    tft.fillScreen(TFT_BLACK);
    tft_goto_ln(0);
    
    //_subpg_inx = 0;
    switch(_curr) {
      case ScreenPage::WLCM:
      case ScreenPage::ERROR: {
        tft.setTextColor(TFT_ORANGE, TFT_BLACK);
        tft.println(_wlcm_msg);
        tft.setTextColor(TFT_WHITE, TFT_BLACK);
        tft_draw_lnxln(1);
        tft.println("TOP/BOT butt: switch screens");
        // -> ends line3
        break;
      }
      case ScreenPage::PM: {
        tft.setTextColor(TFT_ORANGE, TFT_BLACK);
        tft.print("Panasonic: Mass Density (μg/m3)");
        tft.setTextColor(TFT_WHITE, TFT_BLACK);
        tft_draw_lnxln(1);
        // -> ends line2
        break;
      }
      case ScreenPage::PART_CNT: {
        tft.setTextColor(TFT_ORANGE, TFT_BLACK);
        tft.print("Panasonic: Particle Count");
        tft.setTextColor(TFT_WHITE, TFT_BLACK);
        tft_draw_lnxln(1);
        // -> ends line2
        break;
      }
    }
    draw();
  }
}

// ------------------------------------------------------------------------
void clGUI::draw(void) {
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft_goto_ln(2);
  switch(_curr) {
    case ScreenPage::WLCM:
    case ScreenPage::ERROR: {
      tft_goto_ln(3);
      tft.print("Sync: ");
      tft.println(SN_GCJA5.in_sync);
      tft.print("pkts: ");
      tft.println(SN_GCJA5.pkt_cnt);
      tft.print("status: 0x");
      tft.print(SN_GCJA5.status, HEX);
      if(_curr==ScreenPage::ERROR) {
        tft_del_ln(2);
        tft.setTextColor(TFT_RED, TFT_BLACK);
        tft.print("----ERROR----");
      }
      break;
    }
    case ScreenPage::PM: {
      if(_subpg_inx==0) {
        for(auto inx=0; inx<3; ++inx) {
          report_reg_vl(inx, true);
        }
      } else {
        if(_subpg_inx==4) {
          --_subpg_inx;
        }
        report_reg(_subpg_inx+0-1);
      }
      break; 
    }
    case ScreenPage::PART_CNT: {
      if(_subpg_inx==0) {
        for(auto inx=3; inx<9; ++inx) {
          report_reg_vl(inx);
        }
      } else {
        if(_subpg_inx==9) {
          --_subpg_inx;
        }
        report_reg(_subpg_inx+3-1);
      }
      break;
    }
  }
}

// ------------------------------------------------------------------------
void clGUI::report_reg_vl(uint8_t inx, bool adjst) {
  auto vl = SN_GCJA5[inx];
  tft.print(SN_GCJA5_REG_NAMES[inx]);
  switch(_dsply_vl) {
    case 0: {
      if(adjst)
        tft.print(static_cast<double>(vl.curr)/1000.0);
      else
        tft.print(vl.curr);
        
      tft.println(" (curr)");
      break;
    }
    case 1: {
      tft.print(vl.acc);
      tft.println(" (acc)");
      break;
    }
    case 2: {
      tft.print(vl.min);
      tft.println(" (min)");
      break;
    }
    case 3: {
      tft.print(vl.max);
      tft.println(" (max)");
      break;
    }
    case 4: {
      tft.print(vl.avrg);
      tft.println(" (avrg)");
      break;
    }
  }
  
  
}
// ------------------------------------------------------------------------
void clGUI::report_reg(uint8_t inx) {
  auto vl = SN_GCJA5[inx];
  
  //tft.setTextFont(4);
  tft.println(SN_GCJA5_REG_NAMES[inx]);
  tft.print("curr: ");
  tft.print(vl.curr);
  tft.print(", avrg= ");
  tft.println(vl.avrg);
  
  tft.print("min= ");
  tft.print(vl.min);
  tft.print(", max= ");
  tft.println(vl.max);
  
  tft.print("tot=");
  tft.println(vl.acc);
  
  //tft.setTextFont(2);
} 

// ------------------------------------------------------------------------
#define NEXT_PG(CURR, NEXT) case ScreenPage::CURR : set(ScreenPage::NEXT); break
void clGUI::next_pg(void) {
  switch(_curr) {
    NEXT_PG(WLCM, PM);
    NEXT_PG(PM, PART_CNT);
    NEXT_PG(PART_CNT, WLCM);
    default: break;
  }
}
// ------------------------------------------------------------------------
void clGUI::home(void){
  set(ScreenPage::WLCM);
}
// ------------------------------------------------------------------------
#define PREV_PG(CURR, PREV) case ScreenPage::CURR : set(ScreenPage::PREV); break
void clGUI::prev_pg(void){
  switch(_curr) {
    PREV_PG(PM, WLCM);
    PREV_PG(PART_CNT, PM);
    PREV_PG(WLCM, PART_CNT);
    default: break;
  }
}
// ------------------------------------------------------------------------
void clGUI::next_sub_pg(void){
  ++_subpg_inx;
  set(_curr, true);
}
// ------------------------------------------------------------------------
void clGUI::prev_sub_pg(void){
  --_subpg_inx;
  if(_subpg_inx < 0) {
    _subpg_inx = 0;
  }
  set(_curr, true);
}
// ------------------------------------------------------------------------
void clGUI::next_dsply_vl(void){
  ++_dsply_vl;
  if(_dsply_vl > 4) {
    _dsply_vl = 0;
  }
  set(_curr, true);
}
// ------------------------------------------------------------------------
void clGUI::prev_dsply_vl(void){
  --_dsply_vl;
  if(_dsply_vl < 0) {
    _dsply_vl = 4;
  }
  set(_curr, true);
}
// ------------------------------------------------------------------------
void clGUI::main_pg(void){
  _subpg_inx = 0;
  _dsply_vl = 0;
  set(_curr, true);
}
// ------------------------------------------------------------------------
void buttons_hndlr(Button2& btn) {
  if(btn == lft_button) {
    switch (btn.getClickType()) {
      case SINGLE_CLICK:
        GUI.next_pg();
        break;
        
    case DOUBLE_CLICK:
        GUI.next_sub_pg();
        break;
        
      case TRIPLE_CLICK:
        GUI.prev_sub_pg();
        break;
        
      case LONG_CLICK:
        GUI.main_pg();
        break;
    }
  }
  
  if(btn == rgt_button) {
    switch (btn.getClickType()) {
      case SINGLE_CLICK:
        GUI.prev_pg();
        break;
        
      case DOUBLE_CLICK:
        GUI.next_dsply_vl();
        break;
        
      case TRIPLE_CLICK:
        GUI.prev_dsply_vl();
        break;
        
      case LONG_CLICK:
        SN_GCJA5.reset();
        break;
    }
  }
}
// ------------------------------------------------------------------------

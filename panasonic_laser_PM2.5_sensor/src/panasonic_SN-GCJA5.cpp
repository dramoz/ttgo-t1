// ------------------------------------------------------------------------
#include "panasonic_SN-GCJA5.h"
// ------------------------------------------------------------------------
const char* SN_GCJA5_REG_NAMES[] =
{
    (char*) "PM1.0: ",
    (char*) "PM2.5: ",
    (char*) "PM10:  ",
    (char*) "0.3~0.5: ",
    (char*) "0.5~1.0: ",
    (char*) "1.0~2.5: ",
    (char*) "2.5~5.0: ",
    (char*) "5.0~7.5: ",
    (char*) "7.5~10.: "
};
const uint8_t SN_GCJA5_REG_NAMES_size = 9;
const uint8_t PM_CHARS = sizeof(SN_GCJA5_REG_NAMES[0]);
const uint8_t PCNT_CHARS = sizeof(SN_GCJA5_REG_NAMES[SN_GCJA5_REG_NAMES_size-1]);
// ------------------------------------------------------------------------
clSN_GCJA5::clSN_GCJA5(uint8_t serial_num)
  : in_sync(false)
  , pkt_cnt(0)
  , _Serial(serial_num)
  , _pkt_inx(0)
  , _sop(false)
{}

// ------------------------------------------------------------------------
bool clSN_GCJA5::init(int8_t rx_pin, int8_t tx_pin) {
  Serial.begin(SN_GCJA5_UART_BAUDRATE, SN_GCJA5_UART_MODE, rx_pin, tx_pin);
  return sync();
}
// ------------------------------------------------------------------------
bool clSN_GCJA5::sync(void) {
  auto curr_millis = millis();
  while(!in_sync && (millis() - curr_millis < 3000)) {
    if(Serial.available())
    {
      auto ch = uint8_t( Serial.read() );
      if(ch==0x02) {
        _sop = true;
      }
      if(_sop && ch==0x03) {
        _sop = false;
        in_sync = true;
      }
    }
  }
  
  return in_sync;
}
// ------------------------------------------------------------------------
bool clSN_GCJA5::loop(void) {
  uint8_t reg=0;
  uint8_t buff[32];
  
  if(!in_sync) {
      return false;
  }
  
  if(Serial.available()>32)
  {
    static_cast<void>( Serial.read(buff, 32) );
    
    if(buff[0]!=0x02 || buff[31]!=0x03) {
        in_sync = false;
        return false;
    }
    
    ++pkt_cnt;
    
    // STX
    ++reg;
    //PM1.0~PM10
    PM1_0 = BUFu8x4_u32(buff, reg);
    reg += 4;
    PM2_5 = BUFu8x4_u32(buff, reg);
    reg += 4;
    PM10 = BUFu8x4_u32(buff, reg);
    reg += 4;
    
    // Particule counters
    p03_p05_cnt = BUFu8x2_u16(buff, reg);
    reg += 2;
    p05_p10_cnt = BUFu8x2_u16(buff, reg);
    reg += 2;
    p10_p25_cnt = BUFu8x2_u16(buff, reg);
    reg += 2;
    
    reg += 2; // skip x 2
    
    p25_p50_cnt = BUFu8x2_u16(buff, reg);
    reg += 2;
    p50_p75_cnt = BUFu8x2_u16(buff, reg);
    reg += 2;
    p75_p100_cnt = BUFu8x2_u16(buff, reg);
    
    reg += 2; // skip x 2
    
    // Status
    uint8_t fcc = buff[reg];
    ++reg;
    status = buff[reg];
    
    uint8_t calc_fcc = 0x00;
    for(auto inx=1; inx<31;++inx) {
      calc_fcc ^= buff[inx];
    }
    if(calc_fcc != fcc){
      return false;
    }
    return true;
  }
  
  return true;
}

// ------------------------------------------------------------------------
stVal<uint32_t>& clSN_GCJA5::operator[](const int reg) {
  switch(reg) {
    case 0: return PM1_0; break;
    case 1: return PM2_5; break;
    case 2: return PM10; break;
    case 3: return p03_p05_cnt; break;
    case 4: return p05_p10_cnt; break;
    case 5: return p10_p25_cnt; break;
    case 6: return p25_p50_cnt; break;
    case 7: return p50_p75_cnt; break;
    default: return p75_p100_cnt; break;
  }
}

// ------------------------------------------------------------------------
void clSN_GCJA5::reset(void) {
  PM1_0.reset();
  PM2_5.reset();
  PM10.reset();
  p03_p05_cnt.reset();
  p05_p10_cnt.reset();
  p10_p25_cnt.reset();
  p25_p50_cnt.reset();
  p50_p75_cnt.reset();
  p75_p100_cnt.reset();
}

// ------------------------------------------------------------------------
// Panasonic Laser PM2.5 Sensor (SN-GCJA5)
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
#pragma once
#include <Arduino.h>
#include <limits>
// ------------------------------------------------------------------------
// Communications
// ------------------------------------------------------------------------
// UART
#define SN_GCJA5_UART_MODE SERIAL_8E1
#define SN_GCJA5_UART_BAUDRATE 9600
// ------------------------------------------------------------------------
// I2C
// ------------------------------------------------------------------------
#define BUFu8x4_u32(x, i) static_cast<uint32_t>( x[0+i] | (x[1+i] << 8) | (x[2+i] << 16) | (x[3+i] << 24) )
#define BUFu8x2_u16(x, i) static_cast<uint32_t>( x[0+i] | (x[1+i] << 8) )
// ------------------------------------------------------------------------
// Variables
// ------------------------------------------------------------------------
extern const char* SN_GCJA5_REG_NAMES[];
extern const uint8_t SN_GCJA5_REG_NAMES_size;
// -------------------------------------------------
template<class T>
struct stVal {
  T curr;
  T acc;
  T min;
  T max;
  double avrg;
  uint32_t cnt;
  
  stVal(): curr(0), acc(0),
           min(std::numeric_limits<T>::max()),
           max(std::numeric_limits<T>::min()),
           avrg(0.0), cnt(0)
  {}
  
  void reset(void) {
    curr = 0;
    acc = 0;
    min = std::numeric_limits<T>::max();
    max = std::numeric_limits<T>::min();
    avrg = 0.0;
    cnt = 0;
  }
  
  void set(T vl) {
    curr = vl;
    acc += vl;
    ++cnt;
    avrg = static_cast<double>(acc) / static_cast<double>(cnt);
    min = (min > vl) ? (vl) : (min);
    max = (max < vl) ? (vl) : (max);
  }
  stVal& operator=(const T vl) {
    set(vl);
    return *this;
  }
};
// -------------------------------------------------

class clSN_GCJA5 {
  public:
    bool in_sync;
    uint32_t pkt_cnt;
    
    clSN_GCJA5(uint8_t serial_num);
    bool init(int8_t rx_pin, int8_t tx_pin);
    bool sync(void);
    bool loop(void);
    
  private:
    HardwareSerial _Serial;
    uint8_t _pkt_inx;
    bool _sop;
    
  public:
    stVal<uint32_t> PM1_0;
    stVal<uint32_t> PM2_5;
    stVal<uint32_t> PM10;
    stVal<uint32_t> p03_p05_cnt;
    stVal<uint32_t> p05_p10_cnt;
    stVal<uint32_t> p10_p25_cnt;
    stVal<uint32_t> p25_p50_cnt;
    stVal<uint32_t> p50_p75_cnt;
    stVal<uint32_t> p75_p100_cnt;
    
    uint8_t status;
    
    stVal<uint32_t>& operator[](const int reg);
    void reset(void);
};

extern clSN_GCJA5 SN_GCJA5;

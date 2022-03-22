[TOC]

# Overview

The Panasonic SN-GCJA5 is a Particulate Matter (PM) 2.5 sensor that use laser technology to measure air particles as small as 0.3μm  with an ±10% margin from low to high concentrations (~1,000 µgm3). With the aid of an internal fan that generates an air flow, the laser diode scatters the light which is sensed by an internal photo diode. An MCU performs the control and measurements of the particles. The data sensed is available by means of an I2C interface that can be accessed on demand, or a serial UART (output only) interface that provides the measured values every one second.

Summary of characteristics:

- Auto Calibration Function: according to Panasonic, internal alorithms will prolongate the life span of the sensor by monitor the laser diode output light, the photo diode performance and the fan speed which have a natural degradation overtime.
- PM10, PM2.5 and PM1.0 independent readings
- Independent particle counts for the ranges:
  - 0.3-0.5μm
  - 0.5-1.0μm
  - 1.0-2.5μm
  - 2.5-5.0μm
  - 5.0-7.5μm
  - 7.5-10.0μm
- 1 second updates
- Small footprint (37 x 37 x 12 mm)
  - Although I have never test other PM sensors, I have seen them and the Panasonic beat them in size.

- 5 years expected lifespan

Overall, I liked the Panasonic sensor, and altought the documentation is not the best, it was not an obstacle to do some nice tests and R&D.

For a more in depth of theory of operation and to grasp a better understanding on how a laser scattering optical PM sensor works, I recommend to read this nice explanation of the [Sensirion SPS30]( https://sensirion.com/products/product-insights/specialist-articles/particulate-matter-sensing-for-air-quality-measurements/) sensor which uses a similar approach.

## Documentation

The available documentation from Panasonic is limited. The documents can be found at [Panasonic laser-type-sensor](https://na.industrial.panasonic.com/products/sensors/air-quality-gas-flow-sensors/lineup/laser-type-pm-sensor/series/123557/model/123559) website. The available documents are:

- [Panasonic SN-GC Series Sensors Datasheet](https://na.industrial.panasonic.com/file-download/8488): basic features enumeration
- [Panasonic Laser Type PM Sensor Product Specifications](https://na.industrial.panasonic.com/file-download/8683): a more detailed explanation of the sensor
  - IO pins connector
  - Wiring reference
  - Detailed sensor characteristics of measurement accuracy
  - Auto calibration function explanation
  - Drawing
- [Panasonic Laser Type PM Sensor Communication Specifications](https://na.industrial.panasonic.com/file-download/8682): communication protocols
  - I2C communication data format
  - UART TTL speficications
  - Registers information

Overall, the documentation is poor and feels like a poor translation from another language. Although it is enough to connect and use the sensor, a proper application note is missing.

Also, I was disappointed by the lack of information on the sensor connector

I was able to found an [ArduinoUno demo](https://na.industrial.panasonic.com/products/sensors/evaluation-kits/lineup/laser-type-pm-sensor-evaluation-kit), but I was not able to find the Arduino INO file to be downloaded on the board. In any case, I did not put to much effort as the demo required transceivers and other wiring which I did not have at the moment.

## Panasonic Support

At the sensor did came initially without a cable, I contacted Panasonic support - the response what OK and in a timely manner. As I got the cables later from `elemen14`, there was no need to contact them further.

## Community Support

Although there is no that much information out of Panasonic, a google search for `SN-GCJA5` will gave a [Sparkfun library](https://www.arduino.cc/reference/en/libraries/sparkfun-particle-sensor-panasonic-sn-gcja5/) and some other forums. The common denominator on the forums was the problem with the I2C interface while trying to access the sensor. The Sparkfun library is for I2C connection.

# [Particulate Matter](https://en.wikipedia.org/wiki/Particulates)

Particulate Matter (PM) are microscopic particles floating in the air. They are classified by their size, and the classification is based upon a common understanding of human respiratory health impacts.

- **PM10**: <10μm *coarse particulate matter*, can penetrate the deepest part of the lungs such as the bronchioles or alveoli. PM10 can trigger bronchoconstriction in asthmatics patients.
- **PM2.5**: <2.5μm *fine particulate matter* tends to penetrate into the [gas exchange](https://en.wikipedia.org/wiki/Gas_exchange) regions of the lung (alveolus)
- **PM0.1**: <0.μm1 *ultrafine particulate matter* may pass through the lungs to affect other organs.

> Note that this classification does not necessary means a higher health risk, as this would also depends on the type of the particle.

| <img src="https://www.epa.gov/sites/default/files/2016-09/pm2.5_scale_graphic-color_2.jpg" alt="img" style="zoom:50%;" /> |
| :----------------------------------------------------------: |



# Hardware Setup

For this road test, I decide to use the [TTGO-T1](http://www.lilygo.cn/prod_view.aspx?TypeId=50062&Id=1400&FId=t3:50062:3) (aka TTGO T-Display) which can be purchased at [Aliexpress](https://www.aliexpress.com/item/33048962331.html) (Official [LILYGO store](https://lilygo.he.aliexpress.com/store/2090076?spm=a2g0o.detail.1000061.1.59f8142fz8JkSi)). The TTGO-T1 is base on the [ESP32 Espresiff](https://www.espressif.com/en/products/socs/esp32) ([Wikipedia](https://en.wikipedia.org/wiki/ESP32])), a 32bit MCU Tensilica Xtensa LX6 with integrated WiFi, Bluethoot and a lot of peripherals.

There were some particular reasons I selected this board for the roadtest:

- Integrated Display + available libraries
- The  SN-GCJA5 requires a dual voltaje operation
  - 5V power supply
  - 3.3V TTL logic
- User buttons

As the TTGO-T1 works with 3.3V TTL and has a USB port that can provide 5Vdc it was a suitable candidate.

| <img src="https://0.rc.xiniu.com/g4/M00/21/4D/CgAG0mGN4YWAaksbAAGmAvzswEM408.jpg" alt="img" style="zoom:50%;" /> |
| :----------------------------------------------------------: |

## Firmware

I decided to use the UART port to gather all the sensor information. Although I2C is nice when there is limit on available ports on the system, the TTGO-T1 has an extra UART that can be assigned to any port on the board. Furthermore, the comments about the sensor protocol were discouraging.

The SN-GCJA5 UART outputs every second all the internal registers. The only apparent limitation is that the output range of mass concentration ($μg/m^3$) is limited to 2,000$μg/m^3$ on the UART, while it is unlimited on the I2C interface X,XXX$μg/m^3$. However, it should be noticed that the maximum consistency error is expressed as ±10% in the range of values between 35$μg/m^3$ ~ 1,000$μg/m^3$.

The application consist of:

- TTGO-T1 and TTGO-T1 TFT_eSPI drivers wrapped on my own C++ class with some limited functions to make life easier.
- panasonic_SN-GCJA5 library to connect to the UART and unpack the serial data

The application was implemented, compiled and tested under [PlatformIO](https://platformio.org/), a [Visual Code](https://code.visualstudio.com/) plug-in that replaced my old work flow on the [Arduino IDE](https://www.arduino.cc/en/software). The App implementation followed an Arduino approach, and should be easy to port if required. However, I 100% recommend switching to PlatformIO when possible.

The code can be found at https://github.com/dramoz/ttgo-t1, sub-folder [panasonic_laser_PM2.5_sensor](https://github.com/dramoz/ttgo-t1/tree/main/panasonic_laser_PM2.5_sensor).

# GUI



# References

- [Panasonic Laser Type PM Sensor Communication Specifications](https://na.industrial.panasonic.com/file-download/8682)
- [Panasonic Laser Type PM Sensor Product Specifications](https://na.industrial.panasonic.com/file-download/8683)
- [Panasonic SN-GC Series Sensors Datasheet](https://na.industrial.panasonic.com/file-download/8488)
- [Panasonic SN-GCJA5 Sparkfun library](https://www.arduino.cc/reference/en/libraries/sparkfun-particle-sensor-panasonic-sn-gcja5/)
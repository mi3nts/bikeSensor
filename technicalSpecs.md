
# Bike Sensor
This document specifies the technical specifications for the Bike Waggle Sensor.

## 1.0 Sensor Brain : [Particle Photon](https://docs.particle.io/datasheets/photon-(wifi)/photon-datasheet/)
The main platform used: [Particle IO](https://www.particle.io/). For the sensor brain two options were considered.
- Particle [Photon](https://docs.particle.io/datasheets/photon-(wifi)/photon-datasheet/) : Works with Wi - Fi
- Particle [Electron](https://docs.particle.io/datasheets/electron-(cellular)/electron-datasheet/) : Works on Cellular Data 

It was finalized that **Particle Photon** was to be used due to cost constraints. 

### 1.1 Devices Connecting to the Sensor Brain(Particle Photon)
The sensor hub will be responsible in reading the follwing inputs.

| Part Name           | Measurment      | Interface  |
|---------------------|-----------------|------------|
| GPS Module [NEO-6M](https://www.amazon.com/DIYmall-AeroQuad-Antenna-Arduino-Aircraft/dp/B01H5FNA4K/ref=sr_1_3?s=electronics&ie=UTF8&qid=1531853391&sr=1-3&keywords=gps+arduino&dpID=41v5UYJ7tGL&preST=_SY300_QL70_&dpSrc=srch)            | Long/Lat Speed and Altitude  |  RS232 TTL |
| SD Breakout Board [Adafruit-MicroSD](https://www.adafruit.com/product/254)   | Read/Write SD   | SPI mode   |
| RTC Battery [PCF8523](https://www.adafruit.com/product/3295)         | Real Time Clock | I2C        |
| Gyroscope [MPU9250](https://www.amazon.com/HiLetgo%C2%AE-Gyroscope-Acceleration-Accelerator-Magnetometer/dp/B01I1J0Z7Y/ref=pd_sbs_328_3?_encoding=UTF8&pd_rd_i=B01I1J0Z7Y&pd_rd_r=f48e35ee-89fa-11e8-b99a-5517ba108a65&pd_rd_w=6x1U3&pd_rd_wg=LTDJF&pf_rd_i=desktop-dp-sims&pf_rd_m=ATVPDKIKX0DER&pf_rd_p=8702255303818932494&pf_rd_r=VETF7HSKFXE7D8S54H9F&pf_rd_s=desktop-dp-sims&pf_rd_t=40701&psc=1&refRID=VETF7HSKFXE7D8S54H9F)    | 3 axis acceleration - 3 axis orientation - 3 axis magnetometor| I2C|
| Temprature and Humidity Sensor [HTU2ID](https://www.adafruit.com/product/1899)   | Temperature - Relative Humidity         | I2C   |
|Atmospheric Pressure Sensor [BMP280](https://www.amazon.com/Adafruit-BMP280-Barometric-Pressure-Altitude/dp/B01DXCNV2A/ref=sr_1_3?s=electronics&ie=UTF8&qid=1535033187&sr=1-3&keywords=bmp+280&dpID=61oZtQMQ77L&preST=_SX300_QL70_&dpSrc=srch)    | Temperature -  Atmospheric Pressure     | I2C    |
|  Alphasense Device [OPC-N3](http://www.alphasense.com/WEB1213/wp-content/uploads/2018/02/OPC-N3.pdf)  | 24 Bin Counts of 0.38 to 40um Spherical Diemetor - PM1 - PM2.5 - PM10| SPI Mode|
| LCD Display [Smraza 2004](https://www.amazon.com/Smraza-Display-characters-Arduino-MEGA2560/dp/B01K1N4XM6/ref=sr_1_20?s=electronics&ie=UTF8&qid=1531411889&sr=1-20&keywords=lcd+i2c)   | Display Output         | I2C   |


## 2.0 Power Needs

A few more devices are required for a mobile working device. 

- Battery Packs:
    - [Battery Pack 1](https://www.amazon.com/BESWILL-10000MAH-Waterproof-Flashlights-Carabiner/dp/B07BSYZXRT/ref=zg_bs_2407762011_1?_encoding=UTF8&psc=1&refRID=TEYNG1EEWAVTND68WKR0)
    - [Battery Pack 2](https://www.amazon.com/Portable-RAVPower-26800mAh-Protective-Hermitshell/dp/B01KX3854Q/ref=sr_1_10?s=wireless&ie=UTF8&qid=1531859017&sr=1-10&keywords=battery+pack)
    - [Battery Pack 3](https://www.amazon.com/Soluser-13500mAh-Portable-External-Flashlight/dp/B0728NQ1VK/ref=sr_1_4?s=wireless&ie=UTF8&qid=1531858930&sr=1-4&keywords=battery+pack+solar&dpID=51%252BcgtDTTJL&preST=_SY300_QL70_&dpSrc=srch)
- [Power Tester](https://www.amazon.com/DROK-Multimeter-Voltmeter-Capacitance-Charging/dp/B00J3JSEG6/ref=sr_1_4?ie=UTF8&qid=1531407628&sr=8-4&keywords=usb%2Bpower%2Bmeter&th=1)





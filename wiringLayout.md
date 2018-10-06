# The Wiring Layout for  the Bike Waggle Project

## Devices Used
The devices used for the Bike Waggle Project are listed below. 

| Device | Serial data link | Device Address |
|--------|------------------|----------------|
| RTC(PCF8523)		       | I<sup>2</sup>C |   0x68       |
| LCD Module(Smraza 2004)      | I<sup>2</sup>C |   0x3F       |
| Gyroscope(MPU9250)  	       | I<sup>2</sup>C |   0x68/**0x69**  |
| GPS(NEO-6M)   	       | RS232 TTL     	|      -       |
| SD Module(Adafruit)          | SPI          	|      -       |
| Temprature/Humidity(HTU2ID)  | I<sup>2</sup>C |    0x40      |
| Temprature/Pressure(BMP280)  | I<sup>2</sup>C |   **0x76**/0x77  |
| Battery Pack                 |        -       |       -      |

The following are the wiring layouts used for all the devices in use:

### The Real Time Clock(PCF8523)
 
| Photon  |  PCF8523  | 
| ------  | --------- | 
| DO      | SDA       | 
| D1      | SCL       | 
| VIN     | VCC       | 
| GND     | GND       | 

### The Temperature and Humidity Sensor(HTU2ID)

| Photon  |  HTU2ID   | 
| ------  | --------- | 
| DO      | SDA       | 
| D1      | SCL       | 
| 3V3     | VCC       | 
| GND     | GND       | 


### The Temperature and Pressure Sensor(BMP280)

| Photon  |  BMP280   | 
| ------  | --------- | 
| DO      | SDI       | 
| D1      | SCK       | 
| 3V3     | 3V0       | 
| GND     | GND       | 


### The Gyroscope (MPU9250)

| Photon  |  MPU9250  | 
| ------  | --------- | 
| DO      | SDA       | 
| D1      | SCL       | 
| VIN     | VCC       | 
| GND     | GND       | 
| VIN     | ADO       | 

### GPS(NEO-6M)

| Photon  |  NEO-6M   | 
| ------  | --------- | 
| RX      | TX        | 
| TX      | RX        | 
| VIN     | VCC       | 
| GND     | GND       | 


### SD Card(Adafruit MicroSD)

| Photon | SPI Name | SD Reader | 
| ------ | -------- | --------- | 
| A2     | SS       | CS        | 
| A3     | SCK      | SCK       | 
| A4     | MISO     | DO        | 
| A5     | MOSI     | DI        | 
| VIN    |          | 5V        | 
| GND    |          | GND       | 


### Alpha Sensor(OPCN3)

| Photon  |  OPCN3  | 
| ------  | ------- | 
| D2      | MOSI    | 
| D3      | MISO    | 
| D4      | SCK     | 
| D5      | CS/SS   | 
| VIN     | VCC     | 
| GND     | GND     | 



### LCD(-)
| Photon |    LCD    | 
| ------ | --------- | 
| DO     | SDA       | 
| D1     | SCL       | 
| VIN    | VCC       | 
| GND    | GND       | 

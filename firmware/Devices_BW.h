#ifndef __DEVICES_BW_H__
#define __DEVICES_BW_H__

#include "application.h"
#include "Jobs_BW.h"
#include <RTClibrary.h>
#include <HTU21D.h>
#include "quaternionFilters.h"
#include "MPU9250.h"
#include <Adafruit_BMP280.h>
#include <Particle-GPS.h>
#include "Sensorgram.h"
#include <SdFat.h>
#include <LiquidCrystal_I2C_Spark.h>


// LCD
extern LiquidCrystal_I2C *lcd;

//PCF8523
extern RTC_PCF8523 rtc;

//HTU2ID
extern HTU21D htu;
extern Sensorgram HTU2ID_Temperature;
extern Sensorgram HTU2ID_Humidity;

//MPU9250
extern MPU9250 myIMU;

extern bool Device_Stable_MPU9250;
extern Sensorgram MPU9250_Accelaration_X;
extern Sensorgram MPU9250_Accelaration_Y;
extern Sensorgram MPU9250_Accelaration_Z;
extern Sensorgram MPU9250_Gyro_Rate_X;
extern Sensorgram MPU9250_Gyro_Rate_Y;
extern Sensorgram MPU9250_Gyro_Rate_Z;
extern Sensorgram MPU9250_Magnetic_Feild_X;
extern Sensorgram MPU9250_Magnetic_Feild_Y;
extern Sensorgram MPU9250_Magnetic_Feild_Z;
extern Sensorgram MPU9250_Temperature;

//NEO6M

extern Gps _gps ;
extern Timer _timer ;
extern Sensorgram NEO6M_Longitude;
extern Sensorgram NEO6M_Latitude;
extern Sensorgram NEO6M_Altitude;
extern Sensorgram NEO6M_Speed;

//BMP280
extern Adafruit_BMP280 bmp;
extern Sensorgram BMP280_Temperature;
extern Sensorgram BMP280_Pressure;


//Control
extern bool OPCN3_Master   ;
extern bool MPU9250_Master ;
extern bool NEO6M_Master   ;
extern bool BMP280_Master  ;
extern bool HTU2ID_Master  ;

extern bool OPCN3_Online;
extern bool MPU9250_Online;
extern bool NEO6M_Online;
extern bool BMP280_Online;
extern bool HTU2ID_Online;

//OPCN3
#define ALPHA_SLAVE_PIN D5
extern bool flagON;
extern Sensorgram OPCN3_Bin_0 ;
extern Sensorgram OPCN3_Bin_1 ;
extern Sensorgram OPCN3_Bin_2 ;
extern Sensorgram OPCN3_Bin_3 ;
extern Sensorgram OPCN3_Bin_4 ;
extern Sensorgram OPCN3_Bin_5 ;
extern Sensorgram OPCN3_Bin_6 ;
extern Sensorgram OPCN3_Bin_7 ;
extern Sensorgram OPCN3_Bin_8 ;
extern Sensorgram OPCN3_Bin_9 ;
extern Sensorgram OPCN3_Bin_10;
extern Sensorgram OPCN3_Bin_11;
extern Sensorgram OPCN3_Bin_12;
extern Sensorgram OPCN3_Bin_13;
extern Sensorgram OPCN3_Bin_14;
extern Sensorgram OPCN3_Bin_15;
extern Sensorgram OPCN3_Bin_16;
extern Sensorgram OPCN3_Bin_17;
extern Sensorgram OPCN3_Bin_18;
extern Sensorgram OPCN3_Bin_19;
extern Sensorgram OPCN3_Bin_20;
extern Sensorgram OPCN3_Bin_21;
extern Sensorgram OPCN3_Bin_22;
extern Sensorgram OPCN3_Bin_23;
extern Sensorgram OPCN3_Bin_24;

// PM Data
extern Sensorgram OPCN3_PM_1  ;
extern Sensorgram OPCN3_PM_2_5;
extern Sensorgram OPCN3_PM_10 ;
extern Sensorgram OPCN3_Sampling_Time;
// Add the rest of the variables - Can be done





//
extern SdFat sd;
extern SdFile file;
extern String Bike_Waggle_ID ;
extern const uint8_t chipSelect ;
extern File myFile;
extern bool SDFull;

extern int Sensorgram_Publish_Index;
extern int Sensorgram_Reading_Index;
extern Sensorgram OPCN3_Sampling_Time;

extern SdFat sd;
extern SdFile file;
extern String Bike_Waggle_ID ;

extern int Period_When_Stable;
extern unsigned long Loop_Begin_Time;
// PCF8523 Functions
void Initialize_PCF8523_BW();
void Update_PCF8523_BW();

String Get_Current_Time_SD_PCF8523_BW(DateTime RTC_Time);
String Get_Current_Time_HEX_PCF8523_BW(DateTime RTC_Time);
String Get_Current_Time_LCD_PCF8523_BW(DateTime RTC_Time);


// HTU2ID Functions
void Initialize_HTU2ID_BW();
void Read_HTU2ID_BW();

// MPU9250 Functions
void Initialize_MPU9250_BW();
void Read_MPU9250_BW();

// BMP280 Functions
void Initialize_BMP280_BW();
void Read_BMP280_BW();

// NEO-6M Functions
void Initialize_NEO6M_BW();
void onSerialData_BW();
void Read_NEO6M_BW();

// SD Card functions
void Initialize_SD_BW();
String Read_Data_from_SD_BW(String Path);
void Write_Data_To_SD_BW(String Data);
void Write_Data_To_Sensorgram_BW(String Data);
bool Publish_Data_To_Cloud_BW();
void Write_Sensorgram_Packets_To_SD_BW();
void Check_SD_Free_Space_BW();

// OPCN3 Functions
void Initialize_SPI1();
bool Reset_OPCN3_BW();
void beginTransfer();
void endTransfer() ;
bool transferUntilMatch(byte send, byte want, unsigned long timeout) ;
byte transfer(byte send);
void Initialize_OPCN3_BW();
bool Set_Fan_Digital_Pot_State_OPCN3(bool powered) ;
bool Set_Laser_Digital_Pot_State_OPCN3(bool powered) ;
bool Set_Laser_Power_Switch_State_OPCN3(bool powered);
bool Set_Gain_State_OPCN3(bool powered);
bool Read_PM_Only_OPCN3_BW();
bool Read_OPCN3_BW();

bool Get_Device_Stabilty_BW();
void WiFi_Control_BW();

// LCD Functions
void Initialize_LCD_BW();
void Startup_LCD_BW();
void Operational_LCD();



#endif

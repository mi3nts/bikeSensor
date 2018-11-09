// This #include statement was automatically added by the Particle IDE.
#include <SdFat.h>
#include <Particle-GPS.h>
#include <Adafruit_BMP280.h>
#include "quaternionFilters.h"
#include "MPU9250.h"
#include <HTU21D.h>
#include "Sensorgram.h"
#include <RTClibrary.h>
#include "Jobs_BW.h"
#include "Devices_BW.h"
#include <LiquidCrystal_I2C_Spark.h>

//WI-FI CONNECTIVITY
SYSTEM_MODE(SEMI_AUTOMATIC)
SYSTEM_THREAD(ENABLED)


//SD Card
SdFat sd;
SdFile file;
File myFile;
String Bike_Waggle_ID =  "Lakithas_Photon";
const uint8_t chipSelect = A2;
int Sensorgram_Publish_Index;
int Sensorgram_Reading_Index;
bool SDFull = false;

// LCD - May not be used
LiquidCrystal_I2C *lcd;

//PCF8523
RTC_PCF8523 rtc;

//HTU2ID
HTU21D htu = HTU21D();
bool HTU2ID_Online = false;


//MPU9250
MPU9250 myIMU;
bool Device_Stable_MPU9250 = false;
bool MPU9250_Online = false;
bool Device_Stable = false;


//BMP280
bool BMP280_Online = false;
Adafruit_BMP280 bmp;

//NEO-6M
bool NEO6M_Online = false;
Gps _gps = Gps(&Serial1);
Timer _timer = Timer(1, onSerialData_BW);
bool Device_Stable_NEO6M = false;

//OPCN3
bool OPCN3_Online = false;
bool flagON = false;

int Period_When_Stable=250000;// Loop time when Stable in Seconds
int Min_Loop_Time=2000;
unsigned long Loop_Begin_Time;
// Define Master Switches for Each Sensor

bool OPCN3_Master = true;
bool MPU9250_Master = true;
bool NEO6M_Master = true;
bool BMP280_Master = true;
bool HTU2ID_Master = true;


uint8_t Time_Beg ;

bool Function_Registered=false;

// // Sensorgram(String SL, String SID,String SI,String SPID, String SST, String SF , String SD, String SSDT, String SSDS, String SSDP, String SHRF);
// // Data Organization for a Sensogram Object [Length (2B)] [Sensor_ID (2B)] [Sensor_Instance (1B)] [Parameter_ID (1B)] [ Sampling_Time (4B)] [ Data_Format (1B)] [data (xB)] + [SDTime]  [SD_Sensor] [SDParametor] [Humand_Readable_Data]

Sensorgram HTU2ID_Temperature("0004","00A3","00","00","00000000","1E","00","Date_Time","HTU2ID","Temperature","00") ;
Sensorgram HTU2ID_Humidity("0004","00A3","00","01","00000000","1E","00","Date_Time","HTU2ID","Humidity","00") ;

Sensorgram MPU9250_Accelaration_X("0004","00A6","00","00","00000000","1E","00","Date_Time","MPU9250","Acceleration_X","00") ;
Sensorgram MPU9250_Accelaration_Y("0004","00A6","00","01","00000000","1E","00","Date_Time","MPU9250","Acceleration_Y","00") ;
Sensorgram MPU9250_Accelaration_Z("0004","00A6","00","02","00000000","1E","00","Date_Time","MPU9250","Acceleration_Z","00") ;
Sensorgram MPU9250_Gyro_Rate_X("0004","00A6","00","03","00000000","1E","00","Date_Time","MPU9250","Gyro_Rate_X","00") ;
Sensorgram MPU9250_Gyro_Rate_Y("0004","00A6","00","04","00000000","1E","00","Date_Time","MPU9250","Gyro_Rate_Y","00") ;
Sensorgram MPU9250_Gyro_Rate_Z("0004","00A6","00","05","00000000","1E","00","Date_Time","MPU9250","Gyro_Rate_Z","00") ;
Sensorgram MPU9250_Magnetic_Feild_X("0004","00A6","00","06","00000000","1E","00","Date_Time","MPU9250","Mag_Feild_X","00") ;
Sensorgram MPU9250_Magnetic_Feild_Y("0004","00A6","00","07","00000000","1E","00","Date_Time","MPU9250","Mag_Feild_Y","00") ;
Sensorgram MPU9250_Magnetic_Feild_Z("0004","00A6","00","08","00000000","1E","00","Date_Time","MPU9250","Mag_Feild_Z","00") ;
Sensorgram MPU9250_Temperature("0004","00A6","00","09","00000000","1E","00","Date_Time","MPU9250","Temperature","00") ;

// For the BMP 280
Sensorgram BMP280_Temperature("0004","00A4","00","00","00000000","1E","00","Date_Time","BMP280","Temperature","00") ;
Sensorgram    BMP280_Pressure("0004","00A4","00","01","00000000","1E","00","Date_Time","BMP280","Pressure","00") ;

// For the NEO-6M

Sensorgram NEO6M_Longitude("0000","00A2","00","00","00000000","1E","00","Date_Time","NEO6M","Longitude","00") ;
Sensorgram NEO6M_Latitude("0000","00A2","00","01","00000000","1E","00","Date_Time","NEO6M","Latitude","00") ;
Sensorgram NEO6M_Altitude("0000","00A2","00","02","00000000","1E","00","Date_Time","NEO6M","Altitude","00") ;
Sensorgram NEO6M_Speed("0000","00A2","00","03","00000000","1E","00","Date_Time","NEO6M","Speed","00") ;
//

Sensorgram OPCN3_Bin_0("0000","00A1","00","00","00000000","0B","00","Date_Time","OPCN3","Bin0","00") ;
Sensorgram OPCN3_Bin_1("0000","00A1","00","01","00000000","0B","00","Date_Time","OPCN3","Bin1","00") ;
Sensorgram OPCN3_Bin_2("0000","00A1","00","02","00000000","0B","00","Date_Time","OPCN3","Bin2","00") ;
Sensorgram OPCN3_Bin_3("0000","00A1","00","03","00000000","0B","00","Date_Time","OPCN3","Bin3","00") ;
Sensorgram OPCN3_Bin_4("0000","00A1","00","04","00000000","0B","00","Date_Time","OPCN3","Bin4","00") ;
Sensorgram OPCN3_Bin_5("0000","00A1","00","05","00000000","0B","00","Date_Time","OPCN3","Bin5","00") ;
Sensorgram OPCN3_Bin_6("0000","00A1","00","06","00000000","0B","00","Date_Time","OPCN3","Bin6","00") ;
Sensorgram OPCN3_Bin_7("0000","00A1","00","07","00000000","0B","00","Date_Time","OPCN3","Bin7","00") ;
Sensorgram OPCN3_Bin_8("0000","00A1","00","08","00000000","0B","00","Date_Time","OPCN3","Bin8","00") ;
Sensorgram OPCN3_Bin_9("0000","00A1","00","09","00000000","0B","00","Date_Time","OPCN3","Bin9","00") ;
Sensorgram OPCN3_Bin_10("0000","00A1","00","0A","00000000","0B","00","Date_Time","OPCN3","Bin10","00") ;
Sensorgram OPCN3_Bin_11("0000","00A1","00","0B","00000000","0B","00","Date_Time","OPCN3","Bin11","00") ;
Sensorgram OPCN3_Bin_12("0000","00A1","00","0C","00000000","0B","00","Date_Time","OPCN3","Bin12","00") ;
Sensorgram OPCN3_Bin_13("0000","00A1","00","0D","00000000","0B","00","Date_Time","OPCN3","Bin13","00") ;
Sensorgram OPCN3_Bin_14("0000","00A1","00","0E","00000000","0B","00","Date_Time","OPCN3","Bin14","00") ;
Sensorgram OPCN3_Bin_15("0000","00A1","00","0F","00000000","0B","00","Date_Time","OPCN3","Bin15","00") ;
Sensorgram OPCN3_Bin_16("0000","00A1","00","10","00000000","0B","00","Date_Time","OPCN3","Bin16","00") ;
Sensorgram OPCN3_Bin_17("0000","00A1","00","11","00000000","0B","00","Date_Time","OPCN3","Bin17","00") ;
Sensorgram OPCN3_Bin_18("0000","00A1","00","12","00000000","0B","00","Date_Time","OPCN3","Bin18","00") ;
Sensorgram OPCN3_Bin_19("0000","00A1","00","13","00000000","0B","00","Date_Time","OPCN3","Bin19","00") ;
Sensorgram OPCN3_Bin_20("0000","00A1","00","14","00000000","0B","00","Date_Time","OPCN3","Bin20","00") ;
Sensorgram OPCN3_Bin_21("0000","00A1","00","15","00000000","0B","00","Date_Time","OPCN3","Bin21","00") ;
Sensorgram OPCN3_Bin_22("0000","00A1","00","16","00000000","0B","00","Date_Time","OPCN3","Bin22","00") ;
Sensorgram OPCN3_Bin_23("0000","00A1","00","17","00000000","0B","00","Date_Time","OPCN3","Bin23","00") ;
Sensorgram OPCN3_Bin_24("0000","00A1","00","18","00000000","0B","00","Date_Time","OPCN3","Bin24","00") ;

// PM Data
Sensorgram OPCN3_PM_1("0000","00A1","00","20","00000000","1E","00","Date_Time","OPCN3","PM1","00") ;
Sensorgram OPCN3_PM_2_5("0000","00A1","00","21","00000000","1E","00","Date_Time","OPCN3","PM2_5","00") ;
Sensorgram OPCN3_PM_10("0000","00A1","00","22","00000000","1E","00","Date_Time","OPCN3","PM10","00") ;

//
Sensorgram OPCN3_Sampling_Time("0000","00A1","00","30","00000000","0B","00","Date_Time","OPCN3","Sampling_Time","00") ;



void setup() {

Initialize_Serial_BW();
//delay(10000);
Serial.println("Device Started");
Initialize_SPI1();
Initialize_OPCN3_BW();
Initialize_LCD_BW();
Turn_WiFi_On_BW();


Initialize_HTU2ID_BW();
Initialize_MPU9250_BW();
Initialize_BMP280_BW();
Initialize_NEO6M_BW();
Initialize_SD_BW();
delay(1000);
Initialize_PCF8523_BW();
delay(1000);
Update_PCF8523_BW();

}

void loop() {

Loop_Begin_Time = millis();
Time_Beg  = millis();
if (HTU2ID_Master){
Serial.println("Reading HTU2ID");
Read_HTU2ID_BW();}
else{
HTU2ID_Online=false;
}
Serial.println("HTU21D Time:"+String(float(millis()-Time_Beg)/1000));

Time_Beg  = millis();
if (BMP280_Master){
Serial.println("Reading BMP280");
Read_BMP280_BW();}
else{
BMP280_Online=false;
}
Serial.println("BMP280 Time:"+String(float(millis()-Time_Beg)/1000));

Time_Beg  =millis();
if (MPU9250_Master){
Serial.println("Reading MPU9250");
Read_MPU9250_BW();}
else{
MPU9250_Online=false;
}
Serial.println("MPU9250 Time:"+String(float(millis()-Time_Beg)/1000));


Time_Beg  = millis();
if (NEO6M_Master){
Serial.println("Reading NEO6M");
Read_NEO6M_BW();}
else{
NEO6M_Online=false;
}
Serial.println("NEO6M Time:"+String(float(millis()-Time_Beg)/1000));

Time_Beg  = millis();
if (OPCN3_Master){
Serial.println("Reading OPCN3");
Read_OPCN3_BW();}
else{
OPCN3_Online=false;
}
Serial.println("OPCN3 Time:"+String(float(millis()-Time_Beg)/1000));
// print_debug();

Operational_LCD();
Serial.println("Function Registration Check");
Check_Function_Registration();

//Here the sensograms are put into a separate Sensorgram packets
Write_Sensorgram_Packets_To_SD_BW();
WiFi_Control_BW();

// Check_SD_Free_Space_BW();
Serial.println("Changes Made");
// Check_Function_Registration();
Halt_BW();
Serial.println("Final Loop Time:"+String(float(millis()-Loop_Begin_Time)/1000));
}// End of Loop

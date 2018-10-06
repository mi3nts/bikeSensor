
#ifndef __JOBS_BW_H__
#define __JOBS_BW_H__
#include <RTClibrary.h>
#include "application.h"

void Initialize_Serial_BW();
String Float_32_to_Hex(float* yourfloat);
String Int_16_to_Hex(uint16_t* yourint16_t);
String Char_Byte_2_Hex(char val);
String Get_Current_Time_SD(DateTime RTC_Time);
String Get_Current_Time_HEX(DateTime RTC_Time);
String Get_Current_Time_LCD(DateTime RTC_Time);
String Add_Leading_Zeros_8(int val);

float Get_Float_From_Bytes_32_Bit(byte histogram[86],int ind);
uint16_t Get_Int_From_Bytes_16_Bit(byte histogram[86],int ind);
int16_t Get_Int_From_Bytes_16_Bit_2(byte histogram[86],int ind);
int Get_Int_From_Bytes(byte LSB, byte MSB);
String String_to_Hex(String yourString);
String Add_Zeros_2(String Str);
String Add_Zeros_4(String Str);
String Add_Leading_Bits_8(String Bits);



bool Turn_WiFi_On_BW();

void Halt_BW();
void Unlimited_Halt_BW(String Message);

extern bool Function_Registered;
extern unsigned long Loop_Begin_Time;
extern int Min_Loop_Time;
extern int Period_When_Stable;

void Check_Function_Registration();
extern bool OPCN3_Master;
extern bool MPU9250_Master;
extern bool NEO6M_Master;
extern bool BMP280_Master;
extern bool HTU2ID_Master;


extern bool OPCN3_Online;
extern bool MPU9250_Online;
extern bool NEO6M_Online;
extern bool BMP280_Online;
extern bool HTU2ID_Online;

extern String Bike_Waggle_ID;
// Conroller Functions


int Enable(String Message);
int Disable(String Message);
int Rename(String Message);
int PWS(String PW_Stable);
int MLT(String ML_Time);
int Status(String User);

#endif

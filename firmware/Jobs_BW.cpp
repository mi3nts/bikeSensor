#include "Jobs_BW.h"


// Serial Functions #########################################

// Initializing the Serial
void Initialize_Serial_BW(){

while (!Serial) {
    delay(1);
  }
 Serial.begin(9600);
}


String Float_32_to_Hex(float* yourfloat){
unsigned char ch[4];
memcpy(ch,yourfloat,sizeof(float));
String Final= Char_Byte_2_Hex(ch[0]) + Char_Byte_2_Hex(ch[1]) +Char_Byte_2_Hex(ch[2])+ Char_Byte_2_Hex(ch[3]);
return Final;
}


String Int_16_to_Hex(uint16_t* yourint16_t){
unsigned char ch[4];
memcpy(ch,yourint16_t,sizeof(uint16_t));
String Final= Char_Byte_2_Hex(ch[1])+ Char_Byte_2_Hex(ch[0]);
return Final;
}



String Char_Byte_2_Hex(char val){

      String Zeros = "00";
      String Hex = String(int(val),HEX);
      String Final;

    if (Hex.length()<Zeros.length())
        {
        Final  =  Zeros.substring(0,Zeros.length()-Hex.length())+Hex;
        }
        else{
        Final =  Hex;

        }

    return Final;

}





String Get_Current_Time_SD(DateTime RTC_Time){
return String::format("%04d:%02d:%02d:%02d:%02d:%02d",RTC_Time.year(),RTC_Time.month(),RTC_Time.day(),RTC_Time.hour(),RTC_Time.minute(),RTC_Time.second());
}


String Get_Current_Time_HEX(DateTime RTC_Time){
return String(RTC_Time.unixtime(),HEX);
}


String Get_Current_Time_LCD(DateTime RTC_Time){
return String::format("%02d:%02d:%02d",RTC_Time.hour(),RTC_Time.minute(),RTC_Time.second());
}





String String_to_Hex(String yourString){

char Char_Array[yourString.length()];
yourString.toCharArray(Char_Array,yourString.length()+1);

String Final ="";

for (int n=0;n<yourString.length();n++){
Final =   Final+Add_Zeros_2(String(Char_Array[n],HEX));
}

return Final;
}


String Byte_to_Hex(byte yourByte){

return Add_Zeros_2(String(yourByte, HEX));

}

// String Get_Hex_Bytes_From_Array(byte histogram[],int Start, int length){

// String Final="";


// return Add_Zeros_2(String(yourByte, HEX));

// }





float Get_Float_From_Bytes_32_Bit(byte histogram[86],int ind)
{

    float *pm_values = (float *)(&histogram[ind]);

return (*pm_values);

}


uint16_t Get_Int_From_Bytes_16_Bit(byte histogram[86],int ind)
{

uint16_t *pm_values = (uint16_t *)(&histogram[ind]);

return (*pm_values);


}



int Get_Int_From_Bytes(byte LSB, byte MSB)
{
    int Least_Significant_Int = int(LSB);
    int Most_Significant_Int = int(MSB);

    // Combine two bytes into a 16-bit unsigned int
     return ((Most_Significant_Int << 8) | Least_Significant_Int);
}


//




String Add_Zeros_2(String Str){

      String Zeros = "00";
      String Final;

    if (Str.length()<Zeros.length())
        {
        Final  =  Zeros.substring(0,Zeros.length()-Str.length())+Str;
        }
        else{
        Final =  Str;

        }

    return Final;

}




String Add_Zeros_4(String Str){

      String Zeros = "0000";
      String Final;

    if (Str.length()<Zeros.length())
        {
        Final  =  Zeros.substring(0,Zeros.length()-Str.length())+Str;
        }
        else{
        Final =  Str;

        }

    return Final;

}


String Add_Leading_Bits_8(String Bits){

      String  Zeros = "00000000";
      String  Final;

    if (Bits.length()<8)
        {
        Final =  Zeros.substring(0,8-Bits.length())+Bits;
        }
        else{
        Final =  Bits ;

        }

    return Final;

}


String Add_Leading_Zeros_8(int val){

      String Zeros    = "00000000" ;
      String Val_Str  = String(val);
      String Final;

    if (Val_Str.length()<Zeros.length())
        {
        Final  =  Zeros.substring(0,Zeros.length()-Val_Str.length())+Val_Str;
        }
        else{
        Final =  Val_Str;

        }

    return Final;

}


//  delay(15000); // Can make it so the wifi router is off while the device
// works.




bool Turn_WiFi_On_BW()
{
    if (!Particle.connected())   // if not connected to cloud
      {
        Serial.println("Trying to Reconect");
        WiFi.on();
        Particle.connect();        // start a reconnection attempt
        return false;
      }else                         // if already connected
      {
      Serial.println("Already Connected");
      return true;
      }
}





// Photon Halter

void Halt_BW(){
unsigned long Current_Loop_Time = (millis()-Loop_Begin_Time);
Serial.println("Current Loop Time: "+String(Current_Loop_Time));
    if(Current_Loop_Time<Min_Loop_Time){
      Serial.println("Device Delayed");
      delay(Min_Loop_Time-Current_Loop_Time);
    }
}


void Unlimited_Halt_BW(String Message){
Serial.println(Message);
Particle.publish("Error",Message);
delay(1000000);
}



// Controller Functions

void Check_Function_Registration(){

    if(!Function_Registered)
    {
          if(Particle.connected())
            {    SYSTEM_MODE(SEMI_AUTOMATIC)
                 SYSTEM_THREAD(ENABLED)
                 Serial.println("Connection:"+String(Particle.connected()));
                      //if(Particle.function("Device",Get_Device)&&Particle.function("EnblAll",Enable_All)&&Particle.function("DisAll",Disable_All)&&Particle.function("Dis",Disable)&&Particle.function("Ena",Enable)){
                if( \
                   Particle.function("Disable",Disable)&& \
                   Particle.function("Enable",Enable)&& \
                   Particle.function("Rename",Rename)&& \
                   Particle.function("PW_Stable",PWS)&& \
                   Particle.function("ML_Time",MLT)&& \
                   Particle.function("Status",Status) \
                 )
                 {
                  Serial.println("Function Registered");
                  delay(5000);
                  Function_Registered = true;
                 }else{
                 Serial.println("Function Not Registered");
                 }
             }
    }else{
    Serial.println("Function Already Registered");
    }

}



int Enable(String Message){

        if (Message =="ALL"){
          OPCN3_Master   = true;
          MPU9250_Master = true;
          NEO6M_Master   = true;
          BMP280_Master  = true;
          HTU2ID_Master  = true;

          Status("Controller");
              return 1;
        }


      if (Message =="OPCN3"){
            OPCN3_Master   = true;
            Status("Controller");
            return 1;
      }

      if (Message =="MPU9250"){
            MPU9250_Master   = true;
            Status("Controller");
            return 1;
      }

      if (Message =="NEO6M"){
            NEO6M_Master   = true;
            Status("Controller");
            return 1;
      }

      if (Message =="BMP280"){
            BMP280_Master   = true;
            Status("Controller");
            return 1;
      }

      if (Message =="HTU2ID"){
          HTU2ID_Master   = true;
          Status("Controller");
          return 1;
      }
      Status("Controller");
      return -1;
}
//
//
//
int Disable(String Message){

      if (Message =="ALL"){
        OPCN3_Master   = false;
        MPU9250_Master = false;
        NEO6M_Master   = false;
        BMP280_Master  = false;
        HTU2ID_Master  = false;
        Status("Controller");
        return 1;
  }



      if (Message =="OPCN3"){
            OPCN3_Master   = false;
            Status("Controller");
            return 1;
      }

      if (Message =="MPU9250"){
            MPU9250_Master   = false;
            Status("Controller");
            return 1;
      }

      if (Message =="NEO6M"){
            NEO6M_Master   = false;
            Status("Controller");
            return 1;
      }

      if (Message =="BMP280"){
            BMP280_Master   = false;
            Status("Controller");
            return 1;
      }

      if (Message =="HTU2ID"){
          HTU2ID_Master   = false;
          Status("Controller");
          return 1;
      }
      Status("Controller");
      return -1;
}


int Rename(String New_ID){
Bike_Waggle_ID = New_ID;
Serial.println("BW ID changed to "+New_ID);
Status("Controller");
return 1;
}


// this function automagically gets called upon a matching POST request
int MLT(String ML_Time)
{
    int Value = ML_Time.toInt();
    if(Value<=0){
    Serial.println("Minimun Loop Time, Invalid Command");
    Status("Controller");
    return -1;
    }else{
    Min_Loop_Time = Value;
    Serial.println("Minimun Loop Time:"+ String(Min_Loop_Time));
    Status("Controller");
//    Particle.publish("Controller",String(lk));
    return 1;
    }
}


int PWS(String PW_Stable)
{
    int Value = PW_Stable.toInt();
    if(Value<=0){
    Serial.println("Period When Stable, Invalid Command");
    Status("Controller");
    return -1;
    }else{
    Period_When_Stable = Value;
    Serial.println("Period When Stable:"+ String(Period_When_Stable));
    Status("Controller");
//    Particle.publish("Controller",String(lk));
    return 1;
    }
}

int Status(String User)
{
    Serial.println("Status Requested by: "+User);
    String Status_Message = "ID:"+Bike_Waggle_ID \
    +", HTU2ID:"+String(HTU2ID_Master)+String(HTU2ID_Online) \
    +", BMP280:"+String(BMP280_Master)+String(BMP280_Online) \
    +", NEO6M:"+String(NEO6M_Master)+String(NEO6M_Online) \
    +", MPU9250:"+String(MPU9250_Master)+String(MPU9250_Online) \
    +", OPCN3:"+String(OPCN3_Master)+String(OPCN3_Online) \
    +", MLT:"+String(Min_Loop_Time)+", PWS:"+String(Period_When_Stable);
    Serial.println("Status:"+Status_Message);
    Particle.publish("Status",Status_Message);
    delay(1000);
return 1;
}

// void Get_Device_ID_BW(int lk){
// Serial.println("This is the Lakithas Photon");
// Particle.publish("Device_ID","Lakithas_Photon");
// delay(10000);
// }
















// int Get_Int_From_Bytes(byte LSB, byte MSB)
// {
//     int Least_Significant_Int = int(LSB);
//     int Most_Significant_Int = int(MSB);

//     // Combine two bytes into a 16-bit unsigned int
//      return ((Most_Significant_Int << 8) | Least_Significant_Int);
// }

// float Get_Exponent_Single_Precision(String Binary_String){

//     int Power_Pre=0;
//     int Power;
//     float Result;
//     for(int n=(Binary_String.length()-1);n>=0;n--){

//         if(Binary_String.charAt(n)=='1'){
//             Power_Pre = Power_Pre+pow(2,Binary_String.length()-1-n);
//         }
//     }

//     Power = Power_Pre -127;


//     if (Power>16){

//         Result = -1 ;

//     }else if (Power<-16){

//         Result = -2;


//     }else {

//         Result = pow(2,Power);

//     }

//     return Result;

// }










// float Get_Fraction_Single_Precision(String Binary_String){

//     float Final=1;

//     for(int n=0;n<Binary_String.length();n++){

//         if(Binary_String.charAt(n)=='1'){
//             Final = Final+pow(2,-(n+1));
//         }
//     }

//     return Final;

// }




// String Get_Float_From_Bytes_Single_Precision(byte val0, byte val1, byte val2, byte val3)
// {


// String All_Binary = Add_Leading_Bits_8(String(val3,BIN))+ Add_Leading_Bits_8(String(val2,BIN))+ Add_Leading_Bits_8(String(val1,BIN))+ Add_Leading_Bits_8(String(val0,BIN));

// // //String All_Binary ="01000000110110011001100110011010";
//  Serial.println("Binary Value :"+ All_Binary)    ;

// String Sign_Binary = All_Binary.substring(0,1);
// String Exponent_Binary = All_Binary.substring(1,9);
// String Fraction_Binary = All_Binary.substring(9,32);

// float Exponent = Get_Exponent_Single_Precision(Exponent_Binary);
// float Fraction = Get_Fraction_Single_Precision(Fraction_Binary);


// if (Exponent>0){
//     float Float_Value = Exponent*Fraction;
//     return String(Float_Value);
//     }
//     else if (Exponent==-1){
//     Serial.println("VTL")    ;
//     return "VTL";
//     }
//     else if(Exponent==-2)
//     {
//     Serial.println("VTS")    ;
//     return "0";
//     }else{
//     Serial.println("Error")    ;
//     return "Error";
//     }

// }




// bool Compare(byte array1[], byte array2[], int length) {
//   for (int i = 0; i < length; i++){
//     if (array1[i] != array2[i]) {
//       return false;
//     }
//   }
//     return true;
// }

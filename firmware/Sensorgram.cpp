
#include "Sensorgram.h"



Sensorgram::Sensorgram(String SL, String SID,String SI,String SPID, String SST, String SF , String SD, String SSDT, String SSDS, String SSDP, String SHRF) {
    set_Length(SL);
    set_ID(SID);
    set_Sensor_Instance(SI);
    set_Parametor_ID(SPID);
    set_Sampling_Time(SST);
    set_Format(SF);
    set_Data(SD);
    set_SD_Time(SSDT);
    set_SD_Sensor(SSDS);
    set_SD_Parametor(SSDP);
    set_Human_Readable_Form(SHRF);
}



void Sensorgram::set_Length(String L){
    // Set Length
    Length = L;

    }

void Sensorgram::set_ID(String ID){
    // Globally Unique Sensor ID
    Sensor_ID = ID;
}

void Sensorgram::set_Sensor_Instance(String SI){
    // Sensor Instance
    Sensor_Instance = SI;

    }

void Sensorgram::set_Parametor_ID(String PID){
    // Sensor Parametor
    Parametor_ID = PID;
    }

void Sensorgram::set_Sampling_Time(String ST){
    // Setting Sampling Time
    Sampling_Time = ST;
    }

void Sensorgram::set_Format(String F){
    // Setting Sampling Time
    Format = F;
    }

void Sensorgram::set_Data(String D){
    // Set Data
    Data = D;
    }

void Sensorgram::set_SD_Time(String SDT){
    // Set SD Time
    SD_Time = SDT;
    }
void Sensorgram::set_SD_Sensor(String SDS){
    // Set Sensor
    SD_Sensor = SDS;
    }
void Sensorgram::set_SD_Parametor(String SDP){
    // Set Parametor
    SD_Parametor = SDP;
    }

void Sensorgram::set_Human_Readable_Form(String HRF){
    // Set Human Readable Form
    Human_Readable_Form = HRF;
    }



void Sensorgram::Pack(String Hex_Time, String Hex_Value, String SD_T, String H_R_F){

    // The length of Data is automatically Set
    set_Sampling_Time(Hex_Time);
    set_Data(Hex_Value);
    set_SD_Time(SD_T);
    set_Human_Readable_Form(H_R_F);

    // Deciding the number of bits allocated for the data

    String len = String(Hex_Value.length()/2,HEX);

    set_Length(String(Add_Zeros_4(len)));

    //delay(1000);
    }





String Sensorgram::Get_Sensorgram(){
    // Give out the Sensorgram
    return  Length +Sensor_ID+ Sensor_Instance + Parametor_ID + Sampling_Time + Format + Data ;
    }


String Sensorgram::Get_SD_Data(){
    // Give out the Sensorgram
    return  SD_Time+","+SD_Sensor+","+SD_Parametor+","+Human_Readable_Form+";";
    }



String Sensorgram::Get_Human_Readable_Form(){
    // Give the format
    return  Human_Readable_Form;
    }



#ifndef __SENSORGRAM_H__
#define __SENSORGRAM_H__


#include "application.h"
#include "Jobs_BW.h"


class Sensorgram{
    public:
  
    Sensorgram(String SL, String SID,String SI,String SPID, String SST, String SF , String SD, String SSDT, String SSDS, String SSDP, String SHRF);
    void set_Length(String SL);
    void set_ID(String SID);
    void set_Sensor_Instance(String SI);
    void set_Parametor_ID(String SPID);
    void set_Sampling_Time(String SST);
    void set_Format(String SF);
    void set_Data(String SD); 
    void set_SD_Time(String SDT);
    void set_SD_Sensor(String SDS);
    void set_SD_Parametor(String SDP);
    void set_Human_Readable_Form(String HRF);
    void Pack(String Hex_Time, String Hex_Value, String SD_T, String H_R_F);

        String Get_Sensorgram();
        String Get_SD_Data();
        String Get_Human_Readable_Form();



private: 
    String Length;
    String Sensor_ID;
    String Sensor_Instance;
    String Parametor_ID;
    String Sampling_Time;
    String Format;
    String Data;
    String SD_Time;
    String SD_Sensor;
    String SD_Parametor;
    String Human_Readable_Form;

                  }; // Sensogram class ends here
                  



#endif
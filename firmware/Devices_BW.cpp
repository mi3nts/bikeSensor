#include "Devices_BW.h"


// RTC Functionality  #################################################################################################################################

void Initialize_PCF8523_BW(){
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);

    }
  if (! rtc.initialized()) {
    Serial.println("RTC is NOT running!");

   }
 }

void Update_PCF8523_BW(){

if (Particle.connected()){
  rtc.adjust(DateTime(Time.year(), Time.month(), Time.day(), Time.hour(), Time.minute(), Time.second()));
   }
}


String Get_Current_Time_SD_PCF8523_BW(DateTime RTC_Time){
return String::format("%04d:%02d:%02d:%02d:%02d:%02d",RTC_Time.year(),RTC_Time.month(),RTC_Time.day(),RTC_Time.hour(),RTC_Time.minute(),RTC_Time.second());
}


String Get_Current_Time_HEX_PCF8523_BW(DateTime RTC_Time){
return String(RTC_Time.unixtime(),HEX);
}

String Get_Current_Time_Bytes_PCF8523_BW(DateTime RTC_Time){
return String(RTC_Time.unixtime(),HEX);
}



String Get_Current_Time_LCD_PCF8523_BW(DateTime RTC_Time){
return String::format("%02d:%02d:%02d",RTC_Time.hour(),RTC_Time.minute(),RTC_Time.second());
}



// HTU 2ID Functionality  #################################################################################################################################

void Initialize_HTU2ID_BW(){
    	if(! htu.begin()){
	    Serial.println("HTU21D not found");

	}else{
	Serial.println("HTU21D OK");
	HTU2ID_Online =true;
	}
	//delay(1000);
}



void Read_HTU2ID_BW(){

  DateTime now = rtc.now();
  float  HTU2IDTemperature  = htu.readTemperature();
  float  HTU2IDHumidity     = htu.readHumidity();

        if (HTU2IDTemperature>-50&&HTU2IDTemperature<50)
        {
        HTU2ID_Online = true;
        HTU2ID_Temperature.Pack(Get_Current_Time_HEX(now),Float_32_to_Hex(&HTU2IDTemperature),Get_Current_Time_SD(now),String(HTU2IDTemperature));
        HTU2ID_Humidity.Pack(Get_Current_Time_HEX(now),Float_32_to_Hex(&HTU2IDHumidity),Get_Current_Time_SD(now),String(HTU2IDHumidity));
        Write_Data_To_SD_BW(HTU2ID_Temperature.Get_SD_Data());
        Write_Data_To_SD_BW(HTU2ID_Humidity.Get_SD_Data());
        }
}



// MPU9250 Functionality  #################################################################################################################################



void Initialize_MPU9250_BW(){

 // Read the WHO_AM_I register, this is a good test of communication
   byte c = myIMU.readByte(MPU9250_ADDRESS, WHO_AM_I_MPU9250);

  if (c == 0x73) // WHO_AM_I should always be 0x68
  {
    Serial.print("Check 3");
    Serial.println("MPU9250 is online...");

    // Start by performing self test and reporting values
    myIMU.MPU9250SelfTest(myIMU.SelfTest);

    Serial.print("x-axis self test: acceleration trim within : ");
    Serial.print(myIMU.SelfTest[0],1); Serial.println("% of factory value");
    Serial.print("y-axis self test: acceleration trim within : ");
    Serial.print(myIMU.SelfTest[1],1); Serial.println("% of factory value");
    Serial.print("z-axis self test: acceleration trim within : ");
    Serial.print(myIMU.SelfTest[2],1); Serial.println("% of factory value");
    Serial.print("x-axis self test: gyration trim within : ");
    Serial.print(myIMU.SelfTest[3],1); Serial.println("% of factory value");
    Serial.print("y-axis self test: gyration trim within : ");
    Serial.print(myIMU.SelfTest[4],1); Serial.println("% of factory value");
    Serial.print("z-axis self test: gyration trim within : ");
    Serial.print(myIMU.SelfTest[5],1); Serial.println("% of factory value");


    // Calibrate gyro and accelerometers, load biases in bias registers
    myIMU.calibrateMPU9250(myIMU.gyroBias, myIMU.accelBias);

    myIMU.initMPU9250();
    // Initialize device for active mode read of acclerometer, gyroscope, and
    // temperature
    Serial.println("MPU9250 initialized for active data mode....");

    // Read the WHO_AM_I register of the magnetometer, this is a good test of
    // communication
    byte d = myIMU.readByte(AK8963_ADDRESS, WHO_AM_I_AK8963);
    Serial.print("AK8963 "); Serial.print("I AM "); Serial.print(d, HEX);
    Serial.print(" I should be "); Serial.println(0x48, HEX);

    // Get magnetometer calibration from AK8963 ROM
    myIMU.initAK8963(myIMU.magCalibration);
    // Initialize device for active mode read of magnetometer

      //  Serial.println("Calibration values: ");
      Serial.print("X-Axis sensitivity adjustment value ");
      Serial.println(myIMU.magCalibration[0], 2);
      Serial.print("Y-Axis sensitivity adjustment value ");
      Serial.println(myIMU.magCalibration[1], 2);
      Serial.print("Z-Axis sensitivity adjustment value ");
      Serial.println(myIMU.magCalibration[2], 2);


  } // if (c == 0x73)
  else
  {
    Serial.print("Could not connect to MPU9250: 0x");
    Serial.println(c, HEX);
   // while(1) ; // Loop forever if communication doesn't happen
        }
}// End Initialize Gyro




void Read_MPU9250_BW()
{

    if (myIMU.readByte(MPU9250_ADDRESS, INT_STATUS) & 0x01)// Check of the sensor is working
  {
    MPU9250_Online = true;
    myIMU.readAccelData(myIMU.accelCount);  // Read the x/y/z adc values
    myIMU.getAres();

    // Now we'll calculate the accleration value into actual g's
    // This depends on scale being set

    DateTime nowAcc = rtc.now();
    myIMU.ax = (float)myIMU.accelCount[0]*myIMU.aRes; // - accelBias[0];
    myIMU.ay = (float)myIMU.accelCount[1]*myIMU.aRes; // - accelBias[1];
    myIMU.az = (float)myIMU.accelCount[2]*myIMU.aRes; // - accelBias[2];

    myIMU.readGyroData(myIMU.gyroCount);  // Read the x/y/z adc values
    myIMU.getGres();

    // Calculate the gyro value into actual degrees per second
    // This depends on scale being set
    DateTime nowGyro = rtc.now();
    myIMU.gx = (float)myIMU.gyroCount[0]*myIMU.gRes;
    myIMU.gy = (float)myIMU.gyroCount[1]*myIMU.gRes;
    myIMU.gz = (float)myIMU.gyroCount[2]*myIMU.gRes;

    myIMU.readMagData(myIMU.magCount);  // Read the x/y/z adc values
    myIMU.getMres();
    // User environmental x-axis correction in milliGauss, should be
    // automatically calculated
    myIMU.magbias[0] = +470.;
    // User environmental x-axis correction in milliGauss TODO axis??
    myIMU.magbias[1] = +120.;
    // User environmental x-axis correction in milliGauss
    myIMU.magbias[2] = +125.;

    // Calculate the magnetometer values in milliGauss
    // Include factory calibration per data sheet and user environmental
    // corrections
    // Get actual magnetometer value, this depends on scale being set

    DateTime nowMag = rtc.now();
    myIMU.mx = (float)myIMU.magCount[0]*myIMU.mRes*myIMU.magCalibration[0] -
               myIMU.magbias[0];
    myIMU.my = (float)myIMU.magCount[1]*myIMU.mRes*myIMU.magCalibration[1] -
               myIMU.magbias[1];
    myIMU.mz = (float)myIMU.magCount[2]*myIMU.mRes*myIMU.magCalibration[2] -
               myIMU.magbias[2];

        float MPU9250AccelerationX = 1000*myIMU.ax;
        float MPU9250AccelerationY = 1000*myIMU.ay;
        float MPU9250AccelerationZ = 1000*myIMU.az;
        float MPU9250GyroRateX = myIMU.gx;
        float MPU9250GyroRateY = myIMU.gy;
        float MPU9250GyroRateZ = myIMU.gz;
        float MPU9250MagFeildX = myIMU.mx;
        float MPU9250MagFeildY = myIMU.my;
        float MPU9250MagFeildZ = myIMU.mz;
        float MPU9250Temperature    = (((float) myIMU.tempCount) / 333.87 + 21.0);

        //Serial.println("MPU Mag- Temp="+String(MPU9250MagFeildX)+":"+String(MPU9250MagFeildY)+":"+String(MPU9250MagFeildZ)+":"+String(MPU9250Temperature));
        MPU9250_Accelaration_X.Pack(Get_Current_Time_HEX(nowAcc),Float_32_to_Hex(&MPU9250AccelerationX),Get_Current_Time_SD(nowAcc),String(MPU9250AccelerationX));
        MPU9250_Accelaration_Y.Pack(Get_Current_Time_HEX(nowAcc),Float_32_to_Hex(&MPU9250AccelerationY),Get_Current_Time_SD(nowAcc),String(MPU9250AccelerationY));
        MPU9250_Accelaration_Z.Pack(Get_Current_Time_HEX(nowAcc),Float_32_to_Hex(&MPU9250AccelerationZ),Get_Current_Time_SD(nowAcc),String(MPU9250AccelerationZ));

        MPU9250_Gyro_Rate_X.Pack(Get_Current_Time_HEX(nowGyro),Float_32_to_Hex(&MPU9250GyroRateX),Get_Current_Time_SD(nowGyro),String(MPU9250GyroRateX));
        MPU9250_Gyro_Rate_Y.Pack(Get_Current_Time_HEX(nowGyro),Float_32_to_Hex(&MPU9250GyroRateY),Get_Current_Time_SD(nowGyro),String(MPU9250GyroRateY));
        MPU9250_Gyro_Rate_Z.Pack(Get_Current_Time_HEX(nowGyro),Float_32_to_Hex(&MPU9250GyroRateZ),Get_Current_Time_SD(nowGyro),String(MPU9250GyroRateZ));

        MPU9250_Magnetic_Feild_X.Pack(Get_Current_Time_HEX(nowMag),Float_32_to_Hex(&MPU9250MagFeildX),Get_Current_Time_SD(nowMag),String(MPU9250MagFeildX));
        MPU9250_Magnetic_Feild_Y.Pack(Get_Current_Time_HEX(nowMag),Float_32_to_Hex(&MPU9250MagFeildY),Get_Current_Time_SD(nowMag),String(MPU9250MagFeildY));
        MPU9250_Magnetic_Feild_Z.Pack(Get_Current_Time_HEX(nowMag),Float_32_to_Hex(&MPU9250MagFeildZ),Get_Current_Time_SD(nowMag),String(MPU9250MagFeildZ));

        MPU9250_Temperature.Pack(Get_Current_Time_HEX(nowMag),Float_32_to_Hex(&MPU9250Temperature),Get_Current_Time_SD(nowMag),String(MPU9250Temperature));

        Write_Data_To_SD_BW(MPU9250_Accelaration_X.Get_SD_Data());
        Write_Data_To_SD_BW(MPU9250_Accelaration_Y.Get_SD_Data());
        Write_Data_To_SD_BW(MPU9250_Accelaration_Z.Get_SD_Data());

        Write_Data_To_SD_BW(MPU9250_Gyro_Rate_X.Get_SD_Data());
        Write_Data_To_SD_BW(MPU9250_Gyro_Rate_Y.Get_SD_Data());
        Write_Data_To_SD_BW(MPU9250_Gyro_Rate_Z.Get_SD_Data());

        Write_Data_To_SD_BW(MPU9250_Magnetic_Feild_X.Get_SD_Data());
        Write_Data_To_SD_BW(MPU9250_Magnetic_Feild_Y.Get_SD_Data());
        Write_Data_To_SD_BW(MPU9250_Magnetic_Feild_Z.Get_SD_Data());

        Write_Data_To_SD_BW(MPU9250_Temperature.Get_SD_Data());



    Device_Stable_MPU9250 = ((pow(MPU9250GyroRateX,2)+pow(MPU9250GyroRateY,2)+pow(MPU9250GyroRateZ,2)<10));// if this is true the device is stable
    }else
    {
    MPU9250_Online = false;
    Device_Stable_MPU9250 = false;
    }


}// Read MPU 9250


// This Function is dedicated in seeking the device stability Continuosly


bool Get_Device_Stabilty_BW(){
Serial.println("Checking Stabilty");
bool MPU9250_Stablity=false;

   if (myIMU.readByte(MPU9250_ADDRESS, INT_STATUS) & 0x01)// Check of the sensor is working
   {
    myIMU.readGyroData(myIMU.gyroCount);  // Read the x/y/z adc values
    myIMU.getGres();
    // Calculate the gyro value into actual degrees per second
    // This depends on scale being set
    DateTime nowGyro = rtc.now();
    myIMU.gx = (float)myIMU.gyroCount[0]*myIMU.gRes;
    myIMU.gy = (float)myIMU.gyroCount[1]*myIMU.gRes;
    myIMU.gz = (float)myIMU.gyroCount[2]*myIMU.gRes;
        MPU9250_Stablity = ((pow(myIMU.gx,2)+pow(myIMU.gy,2)+pow(myIMU.gz,2)<10));
    }


bool NEO6M_Stablity=false;

Gga gga = Gga(_gps);
Rmc rmc = Rmc(_gps);
bool Parsed = rmc.parse()&&gga.parse();
    if (Parsed&&(gga.positionFixIndicator==1)){
    NEO6M_Stablity =  5>rmc.speedOverGround ;
    }else
    {
        NEO6M_Stablity =true;
    }
Serial.println("Device Stabilty:"+String(MPU9250_Stablity&&NEO6M_Stablity));
return  MPU9250_Stablity&&NEO6M_Stablity;

}



// BMP 280 Functionality  #################################################################################################################################

void Initialize_BMP280_BW(){
    if(!bmp.begin()){
	Serial.println("BMP280 not found");

	}else{
	BMP280_Online =true;
	Serial.println("BMP280 OK");
    }
	//delay(1000);
}


// void Initialize_BMP280_BW(){
// Serial.println("Starting BMP 280");
// bmp.begin();
// }


void Read_BMP280_BW(){

DateTime now = rtc.now();
float BMP180Temperature = bmp.readTemperature();
float BMP180Pressure    = bmp.readPressure();

  if (BMP180Temperature>-50&&BMP180Temperature<50){
      BMP280_Online = true;
      BMP280_Temperature.Pack(Get_Current_Time_HEX(now),Float_32_to_Hex(&BMP180Temperature),Get_Current_Time_SD(now),String(BMP180Temperature));
      BMP280_Pressure.Pack(Get_Current_Time_HEX(now),Float_32_to_Hex(&BMP180Pressure),Get_Current_Time_SD(now),String(BMP180Pressure));
      Write_Data_To_SD_BW(BMP280_Temperature.Get_SD_Data());
      Write_Data_To_SD_BW(BMP280_Pressure.Get_SD_Data());

  }
}


//NEO 6M Functions

void Initialize_NEO6M_BW(){
  _gps.begin(9600);
  _gps.sendCommand(PMTK_SET_NMEA_OUTPUT_ALLDATA);
 _timer.start();
  }


void onSerialData_BW()
{
  _gps.onSerialData();
}

void Read_NEO6M_BW(){

    Gga gga = Gga(_gps);
    Rmc rmc = Rmc(_gps);

//       // ***
//   Serial.print("Data[0] = "); Serial.println(_gps.data[0]);
//   Serial.print("Data[1] = "); Serial.println(_gps.data[1]);
//   Serial.print("Data[2] = "); Serial.println(_gps.data[2]);
//   Serial.print("Data[3] = "); Serial.println(_gps.data[3]);
//   Serial.print("Data[4] = "); Serial.println(_gps.data[4]);
//   Serial.print("Data[5] = "); Serial.println(_gps.data[5]);
//   Serial.print("Data[6] = "); Serial.println(_gps.data[6]);

   DateTime now = rtc.now();
   bool Parsed = rmc.parse()&&gga.parse();

   if (Parsed&&(gga.positionFixIndicator==1)){

        NEO6M_Online = true;
        Serial.println("2) Global Positioning System Fixed Data ($GPGGA)");
        Serial.println("======================================================");
        Serial.print("UTC Time: "); Serial.println(gga.utcTime);
        Serial.print("Latitude: "); Serial.println(gga.latitude);
        Serial.print("North/SouthIndicator: "); Serial.println(gga.northSouthIndicator);
        Serial.print("Longitude: "); Serial.println(gga.longitude);
        Serial.print("East/WestIndicator: "); Serial.println(gga.eastWestIndicator);
        Serial.print("Position Fix Indicator: "); Serial.println(gga.positionFixIndicator);
        Serial.print("Satellites Used: "); Serial.println(gga.satellitesUsed);
        Serial.print("Horizontal Dilution of Precision: "); Serial.println(gga.hdop);
        Serial.print("Altitude: "); Serial.print(gga.altitude); Serial.print(" "); Serial.println(gga.altitudeUnit);
        Serial.print("Geoidal Separation: "); Serial.print(gga.geoidalSeparation); Serial.print(" "); Serial.println(gga.geoidalSeparationUnit);
        Serial.print("Age of Diff. Corr.: "); Serial.println(gga.ageOfDiffCorr);
        Serial.println("");

        Serial.println("1) Recommended Minimum Navigation Information ($GPRMC)");
        Serial.println("======================================================");
        Serial.print("UTC Time: "); Serial.println(rmc.utcTime);
        Serial.print("Latitude: "); Serial.println(rmc.latitude);
        Serial.print("North/SouthIndicator: "); Serial.println(rmc.northSouthIndicator);
        Serial.print("Longitude: "); Serial.println(rmc.longitude);
        Serial.print("East/WestIndicator: "); Serial.println(rmc.eastWestIndicator);
        Serial.print("Speed Over Ground: "); Serial.println(rmc.speedOverGround);
        Serial.print("Course Over Ground: "); Serial.println(rmc.courseOverGround);
        Serial.print("Date: "); Serial.println(rmc.date);
        Serial.print("Magnetic Variation: "); Serial.print(rmc.magneticVariation); Serial.print(" "); Serial.println(rmc.magneticVariationDirection);
        Serial.print("Mode: "); Serial.println(rmc.mode);
        Serial.println("");

        String NEO6MLongitude = rmc.longitude       ;
        String NEO6MLatitude  = rmc.latitude        ;
        float  NEO6MAltitude  = gga.altitude        ;
        float  NEO6MSpeed     = rmc.speedOverGround ;

        //Device_Stable_MPU9250 == (NEO6MSpeed<5);
        NEO6M_Longitude.Pack(Get_Current_Time_HEX(now),String_to_Hex(NEO6MLongitude),Get_Current_Time_SD(now),NEO6MLongitude);
        NEO6M_Latitude.Pack(Get_Current_Time_HEX(now),String_to_Hex(NEO6MLatitude),Get_Current_Time_SD(now),NEO6MLatitude);
        NEO6M_Altitude.Pack(Get_Current_Time_HEX(now),Float_32_to_Hex(&NEO6MAltitude),Get_Current_Time_SD(now),String(NEO6MAltitude));
        NEO6M_Speed.Pack(Get_Current_Time_HEX(now),Float_32_to_Hex(&NEO6MSpeed),Get_Current_Time_SD(now),String(NEO6MSpeed));

        Write_Data_To_SD_BW(NEO6M_Longitude.Get_SD_Data());
        Write_Data_To_SD_BW(NEO6M_Latitude.Get_SD_Data());
        Write_Data_To_SD_BW(NEO6M_Altitude.Get_SD_Data());
        Write_Data_To_SD_BW(NEO6M_Speed.Get_SD_Data());

 }else{

    Serial.println("NO GPS DATA");
          NEO6M_Online = false;
          //Device_Stable_MPU9250 = false;
 }

}


// SD Card functions
void Initialize_SD_BW(){

  if (sd.begin(chipSelect, SPI_HALF_SPEED)) {
    Serial.println("SD Initiated");
    Check_SD_Free_Space_BW();

     }else{
    Serial.println("SD Not Initiated");
   Unlimited_Halt_BW("Please Check Your SD");
  }

  // Create a new folder.
  if (sd.mkdir(Bike_Waggle_ID+"_Data")) {
Serial.println("Data Folder Created");
  }else{
      Serial.println("Data Folder Exists");

  }

  // Create a new folder for sensorgram.
  if (sd.mkdir(Bike_Waggle_ID+"_Sensorgram")) {
        Serial.println("Sensorgram Folder Created");
          }else{
            Serial.println("Sensorgram Folder Exists");
                }
    //Printing Headers
        Serial.println("Data File Opended");
        Write_Data_To_SD_BW("# Project Bike Waggle");
        Write_Data_To_SD_BW("# Version 1.0");
        Write_Data_To_SD_BW("# Bike Waggle - "+Bike_Waggle_ID);
        // DateTime now = rtc.now();
        Write_Data_To_SD_BW("# Logs begin at " + Time.timeStr());
        Write_Data_To_SD_BW("# <Date_Time>,<Sensor>,<Parametor>,<Data>");




// Doing the Index for the Reading File
  if (sd.exists(Bike_Waggle_ID+"_Sensorgram/Reading_Index.txt"))
  {
      Serial.println("File Already Exists");

      Sensorgram_Reading_Index = atoi(Read_Data_from_SD_BW(Bike_Waggle_ID+"_Sensorgram/Reading_Index.txt"));

      } else{

        Serial.println("File Does Not Exist");
        myFile.open(Bike_Waggle_ID+"_Sensorgram/Reading_Index.txt", O_CREAT | O_WRITE | O_AT_END) ;
        Serial.println("Reading Index File Opended");
        myFile.println("1");
        myFile.close();
        Sensorgram_Reading_Index = 1;
    }


    // Doing the Index for the Publish File
  if (sd.exists(Bike_Waggle_ID+"_Sensorgram/Publish_Index.txt"))
  {
      Serial.println("File Already Exists");

      Sensorgram_Publish_Index= atoi(Read_Data_from_SD_BW(Bike_Waggle_ID+"_Sensorgram/Publish_Index.txt"));


      } else{

        Serial.println("Publish File Does Not Exist");
        myFile.open(Bike_Waggle_ID+"_Sensorgram/Publish_Index.txt", O_CREAT | O_WRITE | O_AT_END) ;
        Serial.println("Publish Index File Opended");
        myFile.println("1");
        myFile.close();
        Sensorgram_Publish_Index = 1;
    }




}

String Read_Data_from_SD_BW(String Path){
 // Create a file in Folder1 using a path.
      // re-open the file for reading:
  myFile = sd.open(Path);
  if (myFile) {
    Serial.println("Reading:"+Path);
    // read from the file until there's nothing else in it:
    String Current_Byte;
    String Out = "";
    while (myFile.available()) {
    Current_Byte =(char) myFile.read();
    Out.concat(Current_Byte);
    }
    // close the file:
    myFile.close();
    return Out;
  } else {
    // if the file didn't open, print an error:
    Serial.println("No Such File");
    return "-1";
  }

}



void Write_Data_To_SD_BW(String Data){
 // Create a file in Folder1 using a path.
  if (myFile.open(Bike_Waggle_ID+"_Data/Readings.txt", O_CREAT | O_WRITE | O_AT_END)) {
        Serial.println("Data File Opended");
        myFile.println(Data);
        myFile.close();
    }else
    {
     Serial.println("Data File Not Open");
    }
}



void Write_Data_To_Sensorgram_BW(String Data){
 // Create a file in Folder1 using a path.
   Serial.println("Trying to write Sensorgram");
  if (myFile.open(Bike_Waggle_ID+"_Sensorgram/"+Add_Leading_Zeros_8(Sensorgram_Reading_Index)+".txt", O_CREAT | O_WRITE)) {
        Serial.println("Sensorgram File Opended");
        myFile.print(Data);
        myFile.close();
        Sensorgram_Reading_Index =  Sensorgram_Reading_Index+1;
        //
          if (myFile.open(Bike_Waggle_ID+"_Sensorgram/Reading_Index.txt",  O_CREAT | O_WRITE )){
            Serial.println("Reading Index File Opended");
            myFile.print(String(Sensorgram_Reading_Index));
            myFile.close();
            }



    }else
    {
     Serial.println("Data File Not Open");
    }
}



void Check_SD_Free_Space_BW()
{
Serial.println("Checking SD Size");
uint32_t volFree= sd.vol()->freeClusterCount();
float Free_Space = 0.000512*volFree*sd.vol()->blocksPerCluster();
//delay(2000);
        if(Free_Space<10) {
       Unlimited_Halt_BW("SD Full");
    }
}






void Write_Sensorgram_Packets_To_SD_BW(){
// PAcket 1 consits if HTU2ID, BMP280 and NEO6M
String Packet_1 = "";
String Packet_2 = "";
String Packet_3 = "";
String Packet_4 = "";
String Packet_5 = "";
String Packet_6 = "";
String Packet_7 = "";

    if (HTU2ID_Online){
     Packet_1 =  Packet_1 + HTU2ID_Temperature.Get_Sensorgram()+";"+ HTU2ID_Humidity.Get_Sensorgram()+";";
    }

    if (BMP280_Online){
     Packet_1 =  Packet_1 + BMP280_Temperature.Get_Sensorgram()+";"+BMP280_Pressure.Get_Sensorgram()+";";
    }

    if (NEO6M_Online){
     Packet_1 =  Packet_1 +  NEO6M_Latitude.Get_Sensorgram()+";"+NEO6M_Longitude.Get_Sensorgram()+";"+ NEO6M_Speed.Get_Sensorgram()+";"+NEO6M_Altitude.Get_Sensorgram()+";";
    }

    if(Packet_1.length()>2){
        Write_Data_To_Sensorgram_BW(Packet_1);
    }

    if (MPU9250_Online){
     Packet_2 =  Packet_2 +  MPU9250_Accelaration_X.Get_Sensorgram()+";"+MPU9250_Accelaration_Y.Get_Sensorgram()+";"+ MPU9250_Accelaration_Z.Get_Sensorgram()+";"+MPU9250_Gyro_Rate_X.Get_Sensorgram()+";"+MPU9250_Gyro_Rate_Y.Get_Sensorgram()+";"+MPU9250_Gyro_Rate_Z.Get_Sensorgram()+";";
     Write_Data_To_Sensorgram_BW(Packet_2);
     Packet_3 =  Packet_3 +  MPU9250_Magnetic_Feild_X.Get_Sensorgram()+";"+MPU9250_Magnetic_Feild_Y.Get_Sensorgram()+";"+ MPU9250_Magnetic_Feild_Z.Get_Sensorgram()+";"+MPU9250_Temperature.Get_Sensorgram()+";";
     Write_Data_To_Sensorgram_BW(Packet_3);
    }
    if (OPCN3_Online){
     Packet_4 =  Packet_4 +  OPCN3_Bin_0.Get_Sensorgram()+";"+OPCN3_Bin_1.Get_Sensorgram()+";"+ OPCN3_Bin_2.Get_Sensorgram()+";"+OPCN3_Bin_3.Get_Sensorgram()+";"+OPCN3_Bin_4.Get_Sensorgram()+";"+OPCN3_Bin_5.Get_Sensorgram()+";"+OPCN3_Bin_6.Get_Sensorgram()+";"+OPCN3_Bin_7.Get_Sensorgram()+";";
     Write_Data_To_Sensorgram_BW(Packet_4);
     Packet_5 =  Packet_5 +  OPCN3_Bin_8.Get_Sensorgram()+";"+OPCN3_Bin_9.Get_Sensorgram()+";"+ OPCN3_Bin_10.Get_Sensorgram()+";"+OPCN3_Bin_11.Get_Sensorgram()+";"+OPCN3_Bin_12.Get_Sensorgram()+";"+OPCN3_Bin_13.Get_Sensorgram()+";"+OPCN3_Bin_14.Get_Sensorgram()+";"+OPCN3_Bin_15.Get_Sensorgram()+";";
     Write_Data_To_Sensorgram_BW(Packet_5);
     Packet_6 =  Packet_6 +  OPCN3_Bin_16.Get_Sensorgram()+";"+OPCN3_Bin_17.Get_Sensorgram()+";"+ OPCN3_Bin_18.Get_Sensorgram()+";"+OPCN3_Bin_19.Get_Sensorgram()+";"+OPCN3_Bin_20.Get_Sensorgram()+";"+OPCN3_Bin_21.Get_Sensorgram()+";"+OPCN3_Bin_22.Get_Sensorgram()+";"+OPCN3_Bin_23.Get_Sensorgram()+";";
     Write_Data_To_Sensorgram_BW(Packet_6);
      // Other Data
     Packet_7 =  Packet_7 +   OPCN3_PM_1.Get_Sensorgram()+";"+ OPCN3_PM_2_5.Get_Sensorgram()+";"+ OPCN3_PM_10.Get_Sensorgram()+";"+OPCN3_Sampling_Time.Get_Sensorgram()+";";
     //+OPCN3_Bin_12.Get_Sensorgram()+";"+OPCN3_Bin_13.Get_Sensorgram()+";"+OPCN3_Bin_14.Get_Sensorgram()+";"+OPCN3_Bin_15.Get_Sensorgram()+";";
     Write_Data_To_Sensorgram_BW(Packet_7);
    }
}







bool Publish_Data_To_Cloud_BW(){
 // Create a file in Folder1 using a path.
 Serial.println("Trying to Publish");
 String  Path =Bike_Waggle_ID+"_Sensorgram/"+Add_Leading_Zeros_8(Sensorgram_Publish_Index)+".txt";

 if(Sensorgram_Publish_Index<Sensorgram_Reading_Index)
    {
      if (sd.exists(Path))
      {
        String Sensorgram_Str = Read_Data_from_SD_BW(Path);
        Serial.println("Sensorgram Read for Publishing");
        Serial.println("Sensorgram Read:"+Sensorgram_Str);

            if(!Sensorgram_Str.equals("-1"))
            {
                if(Particle.publish("sensorgram",Sensorgram_Str))
                {
                  Serial.println("Sensorgram Published:");

                    Sensorgram_Publish_Index =  Sensorgram_Publish_Index+1;
                    // Remove files from current directory.
                    sd.remove(Path);

                    if (myFile.open(Bike_Waggle_ID+"_Sensorgram/Publish_Index.txt",  O_CREAT | O_WRITE )){
                        Serial.println("Publish Index File Opened");
                        myFile.print(String(Sensorgram_Publish_Index));
                        myFile.close();
                    }
                }
            }
        }



    return true;
    }else{
    return false;
    }
}



void WiFi_Control_BW() {

    if(Get_Device_Stabilty_BW()&&Turn_WiFi_On_BW()){

        while(Particle.connected()&&Get_Device_Stabilty_BW()){

              if(!Publish_Data_To_Cloud_BW())
              {
                  break;
              }

            delay(1000);// No Need
            Serial.println("Current Loop Time:"+String(millis()-Loop_Begin_Time));
              if(Period_When_Stable <(millis()-Loop_Begin_Time))
              {
              break;
              }

        }
        //WiFi.off();
    }


}



// Alpha Sensor Functions

void Initialize_SPI1()
{
    Serial.println("Initialization");
    delay(2000);
    Serial.println("Init SPI");
    delay(2000);
    SPI1.begin(ALPHA_SLAVE_PIN);
    SPI1.setBitOrder(MSBFIRST);
    SPI1.setDataMode(SPI_MODE1);
    SPI1.setClockSpeed(300000);
 	delay(1000);
	pinMode(ALPHA_SLAVE_PIN, OUTPUT);
    Serial.println("Begining SPI");
}


void beginTransfer() {
    digitalWrite(ALPHA_SLAVE_PIN, LOW);
    delay(1);
}

void endTransfer() {
    delay(1);
    digitalWrite(ALPHA_SLAVE_PIN, HIGH);
}




bool transferUntilMatch(byte send, byte want, unsigned long timeout) {

    unsigned long startTime = millis();
    byte received;
    while (millis() - startTime < timeout) {
         received = transfer(send);
        if (received == want) {
        return true;
        }else if(received == 0x31){
        delay(12);
        }else{
        delay(2100);
        }

    }

    Serial.println("Received Byte: "+String(received,HEX));
    return false;
}


byte transfer(byte send) {
    return SPI1.transfer(send);
}





void Initialize_OPCN3_BW(){

    while (!Set_Fan_Digital_Pot_State_OPCN3(true)) {
        Serial.println("Still trying to turn on device...");
        delay(1000);
    }

    delay(1000);

    while (!Set_Laser_Power_Switch_State_OPCN3(true)) {
        Serial.println("Still trying to turn on device...");
     delay(2500);
    }

    delay(1000);
    Read_PM_Only_OPCN3_BW() ;

}



bool Set_Fan_Digital_Pot_State_OPCN3(bool powered) {
    Serial.println("Setting Fan Digital Pot State");

    beginTransfer();

    if (!transferUntilMatch(0x03, 0xf3, 5000)) {
        Serial.println("Power control timed out.");
        return false;
    }

    delayMicroseconds(10);

    if (powered) {
        transfer(0x03);
    } else {
        transfer(0x02);
    }

    endTransfer();

    return true;
}


bool Set_Laser_Digital_Pot_State_OPCN3(bool powered) {
    Serial.println("Setting Laser Digital Pot State");

    beginTransfer();

    if (!transferUntilMatch(0x03, 0xf3, 5000)) {
        Serial.println("Power control timed out.");
        return false;
    }

    delayMicroseconds(10);

    if (powered) {
        transfer(0x05);
    } else {
        transfer(0x02);
    }

    endTransfer();

    return true;
}



bool Set_Laser_Power_Switch_State_OPCN3(bool powered) {
    Serial.println("Setting Laser Power Switch State");

    beginTransfer();

    if (!transferUntilMatch(0x03, 0xf3, 5000)) {
        Serial.println("Power control timed out.");
        return false;
    }

    delayMicroseconds(10);

    if (powered) {
        transfer(0x07);
    } else {
        transfer(0x02);
    }

    endTransfer();

    return true;
}



bool Set_Gain_State_OPCN3(bool powered) {
    Serial.println("Setting Laser Gain State");

    beginTransfer();

    if (!transferUntilMatch(0x03, 0xf3, 5000)) {
        Serial.println("Power control timed out.");
        return false;
    }

    delayMicroseconds(10);

    if (powered) {
        transfer(0x09);
    } else {
        transfer(0x02);
    }

    endTransfer();

    return true;
}



bool Read_OPCN3_BW() {
    byte Alpha_Readings[86];

    beginTransfer();

    if (!transferUntilMatch(0x30, 0xf3, 5000)) {
        Serial.println("Histogram command failed.");
        return false;
    }

    delay(10);
    DateTime now = rtc.now();
    for (int i = 0; i<86; i++){
        Alpha_Readings[i] = transfer(0x30);
        delayMicroseconds(10);
    }

    endTransfer();

    Serial.println("Histogram:");

    for (int i = 0 ; i<86; i++) {
        Serial.print(Alpha_Readings[i], HEX);

        if (i % 10 == 9) {
            Serial.print("\n");
        } else {
            Serial.print(" ");
        }
    }

    Serial.println();

    Serial.println("--------");

    uint16_t OPCN3Bin0  = Get_Int_From_Bytes_16_Bit(Alpha_Readings,0);
    uint16_t OPCN3Bin1  = Get_Int_From_Bytes_16_Bit(Alpha_Readings,2);
    uint16_t OPCN3Bin2  = Get_Int_From_Bytes_16_Bit(Alpha_Readings,4);
    uint16_t OPCN3Bin3  = Get_Int_From_Bytes_16_Bit(Alpha_Readings,6);
    uint16_t OPCN3Bin4  = Get_Int_From_Bytes_16_Bit(Alpha_Readings,8);
    uint16_t OPCN3Bin5  = Get_Int_From_Bytes_16_Bit(Alpha_Readings,10);
    uint16_t OPCN3Bin6  = Get_Int_From_Bytes_16_Bit(Alpha_Readings,12);
    uint16_t OPCN3Bin7  = Get_Int_From_Bytes_16_Bit(Alpha_Readings,14);
    uint16_t OPCN3Bin8  = Get_Int_From_Bytes_16_Bit(Alpha_Readings,16);
    uint16_t OPCN3Bin9  = Get_Int_From_Bytes_16_Bit(Alpha_Readings,18);
    uint16_t OPCN3Bin10 = Get_Int_From_Bytes_16_Bit(Alpha_Readings,20);
    uint16_t OPCN3Bin11 = Get_Int_From_Bytes_16_Bit(Alpha_Readings,22);
    uint16_t OPCN3Bin12 = Get_Int_From_Bytes_16_Bit(Alpha_Readings,24);
    uint16_t OPCN3Bin13 = Get_Int_From_Bytes_16_Bit(Alpha_Readings,26);
    uint16_t OPCN3Bin14 = Get_Int_From_Bytes_16_Bit(Alpha_Readings,28);
    uint16_t OPCN3Bin15 = Get_Int_From_Bytes_16_Bit(Alpha_Readings,30);
    uint16_t OPCN3Bin16 = Get_Int_From_Bytes_16_Bit(Alpha_Readings,32);
    uint16_t OPCN3Bin17 = Get_Int_From_Bytes_16_Bit(Alpha_Readings,34);
    uint16_t OPCN3Bin18 = Get_Int_From_Bytes_16_Bit(Alpha_Readings,36);
    uint16_t OPCN3Bin19 = Get_Int_From_Bytes_16_Bit(Alpha_Readings,38);
    uint16_t OPCN3Bin20 = Get_Int_From_Bytes_16_Bit(Alpha_Readings,40);
    uint16_t OPCN3Bin21 = Get_Int_From_Bytes_16_Bit(Alpha_Readings,42);
    uint16_t OPCN3Bin22 = Get_Int_From_Bytes_16_Bit(Alpha_Readings,44);
    uint16_t OPCN3Bin23 = Get_Int_From_Bytes_16_Bit(Alpha_Readings,46);
    float   OPCN3PM1             = Get_Float_From_Bytes_32_Bit(Alpha_Readings,60);
    float   OPCN3PM2_5           = Get_Float_From_Bytes_32_Bit(Alpha_Readings,64);
    float   OPCN3PM10            = Get_Float_From_Bytes_32_Bit(Alpha_Readings,68);
    uint16_t OPCN3SamplingTime    = 20*Get_Int_From_Bytes_16_Bit(Alpha_Readings,52);
    uint16_t OPCN3Temperature     = Get_Int_From_Bytes_16_Bit(Alpha_Readings,56);
    uint16_t OPCN3Humidity        = 2*Get_Int_From_Bytes_16_Bit(Alpha_Readings,58);

    Serial.println("Bin 0  :"+String(OPCN3Bin0));
    Serial.println("Bin 1  :"+String(OPCN3Bin1));
    Serial.println("Bin 2  :"+String(OPCN3Bin2));
    Serial.println("Bin 3  :"+String(OPCN3Bin3));
    Serial.println("Bin 4  :"+String(OPCN3Bin4));
    Serial.println("Bin 5  :"+String(OPCN3Bin5));
    Serial.println("Bin 6  :"+String(OPCN3Bin6));
    Serial.println("Bin 7  :"+String(OPCN3Bin7));
    Serial.println("Bin 8  :"+String(OPCN3Bin8));
    Serial.println("Bin 9  :"+String(OPCN3Bin9));
    Serial.println("Bin 10 :"+String(OPCN3Bin10));
    Serial.println("Bin 11 :"+String(OPCN3Bin11));
    Serial.println("Bin 12 :"+String(OPCN3Bin12));
    Serial.println("Bin 13 :"+String(OPCN3Bin13));
    Serial.println("Bin 14 :"+String(OPCN3Bin14));
    Serial.println("Bin 15 :"+String(OPCN3Bin15));
    Serial.println("Bin 16 :"+String(OPCN3Bin16));
    Serial.println("Bin 17 :"+String(OPCN3Bin17));
    Serial.println("Bin 18 :"+String(OPCN3Bin18));
    Serial.println("Bin 19 :"+String(OPCN3Bin19));
    Serial.println("Bin 20 :"+String(OPCN3Bin20));
    Serial.println("Bin 21 :"+String(OPCN3Bin21));
    Serial.println("Bin 22 :"+String(OPCN3Bin22));
    Serial.println("Bin 23 :"+String(OPCN3Bin23));
    Serial.println("PM 1    :"+String(OPCN3PM1));
    Serial.println("PM 2.5  :"+String(OPCN3PM2_5));
    Serial.println("PM 10   :"+String(OPCN3PM10));
    Serial.println("Temperature  :"+String(OPCN3Temperature));
    Serial.println("Humidity     :"+String(OPCN3Humidity));
    Serial.println("Sampling Time:"+String(OPCN3SamplingTime));
    Serial.println("Power:"+String(Get_Int_From_Bytes_16_Bit(Alpha_Readings,82)));

   if(Get_Int_From_Bytes_16_Bit(Alpha_Readings,82) >= 500){
    OPCN3_Online = true;
     // Put all the pack functions here
    OPCN3_Bin_0.Pack(Get_Current_Time_HEX(now),Int_16_to_Hex(&OPCN3Bin0),Get_Current_Time_SD(now),String(OPCN3Bin0));
    OPCN3_Bin_1.Pack(Get_Current_Time_HEX(now),Int_16_to_Hex(&OPCN3Bin1),Get_Current_Time_SD(now),String(OPCN3Bin1));
    OPCN3_Bin_2.Pack(Get_Current_Time_HEX(now),Int_16_to_Hex(&OPCN3Bin2),Get_Current_Time_SD(now),String(OPCN3Bin2));
    OPCN3_Bin_3.Pack(Get_Current_Time_HEX(now),Int_16_to_Hex(&OPCN3Bin3),Get_Current_Time_SD(now),String(OPCN3Bin3));
    OPCN3_Bin_4.Pack(Get_Current_Time_HEX(now),Int_16_to_Hex(&OPCN3Bin4),Get_Current_Time_SD(now),String(OPCN3Bin4));
    OPCN3_Bin_5.Pack(Get_Current_Time_HEX(now),Int_16_to_Hex(&OPCN3Bin5),Get_Current_Time_SD(now),String(OPCN3Bin5));
    OPCN3_Bin_6.Pack(Get_Current_Time_HEX(now),Int_16_to_Hex(&OPCN3Bin6),Get_Current_Time_SD(now),String(OPCN3Bin6));
    OPCN3_Bin_7.Pack(Get_Current_Time_HEX(now),Int_16_to_Hex(&OPCN3Bin7),Get_Current_Time_SD(now),String(OPCN3Bin7));
    OPCN3_Bin_8.Pack(Get_Current_Time_HEX(now),Int_16_to_Hex(&OPCN3Bin8),Get_Current_Time_SD(now),String(OPCN3Bin8));
    OPCN3_Bin_9.Pack(Get_Current_Time_HEX(now),Int_16_to_Hex(&OPCN3Bin9),Get_Current_Time_SD(now),String(OPCN3Bin9));
    OPCN3_Bin_10.Pack(Get_Current_Time_HEX(now),Int_16_to_Hex(&OPCN3Bin10),Get_Current_Time_SD(now),String(OPCN3Bin10));
    OPCN3_Bin_11.Pack(Get_Current_Time_HEX(now),Int_16_to_Hex(&OPCN3Bin11),Get_Current_Time_SD(now),String(OPCN3Bin11));
    OPCN3_Bin_12.Pack(Get_Current_Time_HEX(now),Int_16_to_Hex(&OPCN3Bin12),Get_Current_Time_SD(now),String(OPCN3Bin12));
    OPCN3_Bin_13.Pack(Get_Current_Time_HEX(now),Int_16_to_Hex(&OPCN3Bin13),Get_Current_Time_SD(now),String(OPCN3Bin13));
    OPCN3_Bin_14.Pack(Get_Current_Time_HEX(now),Int_16_to_Hex(&OPCN3Bin14),Get_Current_Time_SD(now),String(OPCN3Bin14));
    OPCN3_Bin_15.Pack(Get_Current_Time_HEX(now),Int_16_to_Hex(&OPCN3Bin15),Get_Current_Time_SD(now),String(OPCN3Bin15));
    OPCN3_Bin_16.Pack(Get_Current_Time_HEX(now),Int_16_to_Hex(&OPCN3Bin16),Get_Current_Time_SD(now),String(OPCN3Bin16));
    OPCN3_Bin_17.Pack(Get_Current_Time_HEX(now),Int_16_to_Hex(&OPCN3Bin17),Get_Current_Time_SD(now),String(OPCN3Bin17));
    OPCN3_Bin_18.Pack(Get_Current_Time_HEX(now),Int_16_to_Hex(&OPCN3Bin18),Get_Current_Time_SD(now),String(OPCN3Bin18));
    OPCN3_Bin_19.Pack(Get_Current_Time_HEX(now),Int_16_to_Hex(&OPCN3Bin19),Get_Current_Time_SD(now),String(OPCN3Bin19));
    OPCN3_Bin_20.Pack(Get_Current_Time_HEX(now),Int_16_to_Hex(&OPCN3Bin20),Get_Current_Time_SD(now),String(OPCN3Bin20));
    OPCN3_Bin_21.Pack(Get_Current_Time_HEX(now),Int_16_to_Hex(&OPCN3Bin21),Get_Current_Time_SD(now),String(OPCN3Bin21));
    OPCN3_Bin_22.Pack(Get_Current_Time_HEX(now),Int_16_to_Hex(&OPCN3Bin22),Get_Current_Time_SD(now),String(OPCN3Bin22));
    OPCN3_Bin_23.Pack(Get_Current_Time_HEX(now),Int_16_to_Hex(&OPCN3Bin23),Get_Current_Time_SD(now),String(OPCN3Bin23));

    OPCN3_PM_1.Pack(Get_Current_Time_HEX(now),Float_32_to_Hex(&OPCN3PM1),Get_Current_Time_SD(now),String(OPCN3PM1));
    OPCN3_PM_2_5.Pack(Get_Current_Time_HEX(now),Float_32_to_Hex(&OPCN3PM2_5),Get_Current_Time_SD(now),String(OPCN3PM2_5));
    OPCN3_PM_10.Pack(Get_Current_Time_HEX(now),Float_32_to_Hex(&OPCN3PM10),Get_Current_Time_SD(now),String(OPCN3PM10));
    OPCN3_Sampling_Time.Pack(Get_Current_Time_HEX(now),Int_16_to_Hex(&OPCN3SamplingTime),Get_Current_Time_SD(now),String(OPCN3SamplingTime));


    Write_Data_To_SD_BW(OPCN3_Bin_0.Get_SD_Data());
    Write_Data_To_SD_BW(OPCN3_Bin_1.Get_SD_Data());
    Write_Data_To_SD_BW(OPCN3_Bin_2.Get_SD_Data());
    Write_Data_To_SD_BW(OPCN3_Bin_3.Get_SD_Data());
    Write_Data_To_SD_BW(OPCN3_Bin_4.Get_SD_Data());
    Write_Data_To_SD_BW(OPCN3_Bin_5.Get_SD_Data());
    Write_Data_To_SD_BW(OPCN3_Bin_6.Get_SD_Data());
    Write_Data_To_SD_BW(OPCN3_Bin_7.Get_SD_Data());
    Write_Data_To_SD_BW(OPCN3_Bin_8.Get_SD_Data());
    Write_Data_To_SD_BW(OPCN3_Bin_9.Get_SD_Data());
    Write_Data_To_SD_BW(OPCN3_Bin_10.Get_SD_Data());
    Write_Data_To_SD_BW(OPCN3_Bin_11.Get_SD_Data());
    Write_Data_To_SD_BW(OPCN3_Bin_12.Get_SD_Data());
    Write_Data_To_SD_BW(OPCN3_Bin_13.Get_SD_Data());
    Write_Data_To_SD_BW(OPCN3_Bin_14.Get_SD_Data());
    Write_Data_To_SD_BW(OPCN3_Bin_15.Get_SD_Data());
    Write_Data_To_SD_BW(OPCN3_Bin_16.Get_SD_Data());
    Write_Data_To_SD_BW(OPCN3_Bin_17.Get_SD_Data());
    Write_Data_To_SD_BW(OPCN3_Bin_18.Get_SD_Data());
    Write_Data_To_SD_BW(OPCN3_Bin_19.Get_SD_Data());
    Write_Data_To_SD_BW(OPCN3_Bin_20.Get_SD_Data());
    Write_Data_To_SD_BW(OPCN3_Bin_21.Get_SD_Data());
    Write_Data_To_SD_BW(OPCN3_Bin_22.Get_SD_Data());
    Write_Data_To_SD_BW(OPCN3_Bin_23.Get_SD_Data());

    Write_Data_To_SD_BW(OPCN3_PM_1.Get_SD_Data());
    Write_Data_To_SD_BW(OPCN3_PM_2_5.Get_SD_Data());
    Write_Data_To_SD_BW(OPCN3_PM_10.Get_SD_Data());
    Write_Data_To_SD_BW(OPCN3_Sampling_Time.Get_SD_Data());

   return true;}
    else{
    OPCN3_Online = false;
    return false;
    }
}




bool Read_PM_Only_OPCN3_BW() {
    byte Alpha_Readings[14];

    beginTransfer();

    if (!transferUntilMatch(0x32, 0xf3, 1000)) {
        Serial.println("Histogram command failed.");
        return false;
    }

    delay(10);

    for (int i = 0; i<14; i++){
        Alpha_Readings[i] = transfer(0x32);
        delayMicroseconds(10);
    }

    endTransfer();

    if(!(Alpha_Readings[0]== 0x00)){
    return true;
    Serial.println("Resetting Histogram");

    }
    else{
    return false;
    }
}

// LCD Display
void Initialize_LCD_BW(){

Serial.println("Initializing LCD");
   lcd = new LiquidCrystal_I2C(0x3F /*address*/, 20/*columns*/, 4/*rows*/);
   lcd->init();
   lcd->backlight();
   lcd->clear();

Startup_LCD_BW();

}


void Startup_LCD_BW(){
  Serial.println("Intro LCD");

   lcd->setCursor(5 /*columns*/,1 /*rows*/);
   lcd->print("BIKE");
   delay(1000) ;
   lcd->setCursor(8 /*columns*/,2 /*rows*/);
   lcd->print("WAGGLE");
   delay(1000) ;
   lcd->clear();


      for (int count=0;count<=19;count++)
         {
         lcd->setCursor(count /*columns*/,0 /*rows*/);
         lcd->print("#");
         lcd->setCursor(count /*columns*/,1 /*rows*/);
         lcd->print("#");
         lcd->setCursor(count /*columns*/,2 /*rows*/);
         lcd->print("#");
         lcd->setCursor(count /*columns*/,3/*rows*/);
         lcd->print("#");
         delay(40);
         lcd->clear();
        }

      for (int count=19;count>=0;count--)
      {
         lcd->setCursor(count /*columns*/,0 /*rows*/);
         lcd->print("#");
         lcd->setCursor(count /*columns*/,1 /*rows*/);
         lcd->print("#");
         lcd->setCursor(count /*columns*/,2 /*rows*/);
         lcd->print("#");
         lcd->setCursor(count /*columns*/,3/*rows*/);
         lcd->print("#");
         delay(40);
         lcd->clear();
      }



      for (int count=0;count<=19;count++)
      {
         lcd->setCursor(count /*columns*/,0 /*rows*/);
         lcd->print("#");
         lcd->setCursor(count /*columns*/,1 /*rows*/);
         lcd->print("#");
         lcd->setCursor(count /*columns*/,2 /*rows*/);
         lcd->print("#");
         lcd->setCursor(count /*columns*/,3 /*rows*/);
         lcd->print("#");
         delay(40);
         lcd->clear();
      }

      for (int count=19;count>=0;count--)
      {
         lcd->setCursor(count /*columns*/,0 /*rows*/);
         lcd->print("#");
         lcd->setCursor(count /*columns*/,1 /*rows*/);
         lcd->print("#");
         lcd->setCursor(count /*columns*/,2 /*rows*/);
         lcd->print("#");
         lcd->setCursor(count /*columns*/,3 /*rows*/);
         lcd->print("#");
         delay(40);
         lcd->clear();
      }



        lcd->setCursor(5 /*columns*/,1 /*rows*/);
         lcd->print("BIKE");
         delay(1000) ;

         lcd->setCursor(8 /*columns*/,2 /*rows*/);
         lcd->print("WAGGLE");
         delay(1000) ;
         lcd->clear();

         // Give Device Data
         lcd->setCursor(0 /*columns*/,0 /*rows*/);
         lcd->print("Project Bike Waggle");
         lcd->setCursor(0 /*columns*/,1 /*rows*/);
         lcd->print("Version 1.0");
         lcd->setCursor(0 /*columns*/,2 /*rows*/);
         lcd->print("Initializing");
         lcd->setCursor(3 /*columns*/,3 /*rows*/);
         lcd->print("Lakithas Photon");
         delay(1000) ;
         lcd->clear();
        delay(1000) ;
         lcd->setCursor(5 /*columns*/,1 /*rows*/);
          lcd->print("BIKE");
          delay(1000) ;

          lcd->setCursor(8 /*columns*/,2 /*rows*/);
          lcd->print("WAGGLE");
        //  delay(1000) ;

 }


void Operational_LCD(){

      DateTime now = rtc.now();
      lcd->clear();
      lcd->setCursor(0 /*columns*/,0 /*rows*/);
      lcd->print("Bike Waggle|"+  Get_Current_Time_LCD_PCF8523_BW(now));
      lcd->setCursor(6 /*columns*/,1 /*rows*/);
      lcd->print("|");
      lcd->setCursor(6 /*columns*/,2 /*rows*/);
      lcd->print("|");
      lcd->setCursor(6 /*columns*/,3 /*rows*/);
      lcd->print("|");
      lcd->setCursor(13 /*columns*/,1 /*rows*/);
      lcd->print("|");
      lcd->setCursor(13 /*columns*/,2 /*rows*/);
      lcd->print("|");
      lcd->setCursor(13 /*columns*/,3 /*rows*/);
      lcd->print("|");

    if (OPCN3_Online)
    {
        lcd->setCursor(0 /*columns*/,1 /*rows*/);
        lcd->print(OPCN3_PM_1.Get_Human_Readable_Form().substring(0,6));
        lcd->setCursor(7 /*columns*/,1 /*rows*/);
        lcd->print(OPCN3_PM_2_5.Get_Human_Readable_Form().substring(0,6));
        lcd->setCursor(14 /*columns*/,1/*rows*/);
        lcd->print(OPCN3_PM_10.Get_Human_Readable_Form().substring(0,6));
    }
    //
    // Serial.println("HTU online:"+String(HTU2ID_Online));
    // Serial.println("HTU Temperature:"+HTU2ID_Temperature.Get_Human_Readable_Form());


    if (HTU2ID_Online)
    {
        lcd->setCursor(0 /*columns*/,2 /*rows*/);  /* code */
        lcd->print(HTU2ID_Temperature.Get_Human_Readable_Form().substring(0,6));
        lcd->setCursor(7 /*columns*/,2 /*rows*/);
        lcd->print(HTU2ID_Humidity.Get_Human_Readable_Form().substring(0,6));
        lcd->setCursor(14 /*columns*/,2/*rows*/);
    }

   //
   // Serial.println("BMP280 online:"+String(BMP280_Online));
   // Serial.println("BMP280 Pressure:"+BMP280_Pressure.Get_Human_Readable_Form());


    if (BMP280_Online)
    {
       lcd->setCursor(14 /*columns*/,2/*rows*/);
       lcd->print(BMP280_Pressure.Get_Human_Readable_Form().substring(0,6));
    }

    if (NEO6M_Online)
    {
        lcd->setCursor(0 /*columns*/,3 /*rows*/);
        lcd->print(NEO6M_Altitude.Get_Human_Readable_Form().substring(0,6));
        lcd->setCursor(7 /*columns*/,3 /*rows*/);
        lcd->print(NEO6M_Speed.Get_Human_Readable_Form().substring(0,6));
    }

    if (MPU9250_Online)
    {
      lcd->setCursor(14 /*columns*/,3/*rows*/);
      lcd->print(MPU9250_Accelaration_Z.Get_Human_Readable_Form().substring(0,6));
    }


}

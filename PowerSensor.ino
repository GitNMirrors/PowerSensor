#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS A0

#define MASS  0.033 //mass in Kg
#define C 900       //specific heat capacity aluminium J/Kg * °K 

#define DEBUG 0 
#define CALIBRATE 0
#define REFERENCE 82.0

//Calculates Watt from a temperature difference and time
float GetWatts(float dK, unsigned int dt){

  float watt = ((C*MASS)*dK)/((float)(dt/1000));
  return (watt); 
}

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

#define MAXDATA 30 
struct
{
  int id;
  DeviceAddress addr;
  float temp;
  unsigned long sampletime;
  float dTemp;
  unsigned int  dTime;
} Data[MAXDATA];

uint8_t ringbufpointer = 0;

uint8_t next(){
  uint8_t ret = 0;
  if(ringbufpointer < MAXDATA-1){
    ret = ringbufpointer+ 1;
  }
  return ret;
}

uint8_t prev(){
  uint8_t ret = MAXDATA-1;
  if(ringbufpointer > 0){
    ret = ringbufpointer- 1;
  }
  return ret;
}
uint8_t prevof(uint8_t pointer){
  uint8_t ret = MAXDATA-1;
  if(pointer > 0){
    ret = pointer- 1;
  }
  return ret;
}


float maxtemp = 0;
float starttemp = 0;
void getTemp(){
    Data[next()].temp = sensors.getTempCByIndex(0);
    Data[next()].sampletime = millis();
    Data[next()].dTemp = Data[next()].temp - Data[ringbufpointer].temp;
    Data[next()].dTime = Data[next()].sampletime - Data[ringbufpointer].sampletime;
    if( Data[next()].temp > maxtemp)  maxtemp = Data[next()].temp;
    ringbufpointer = next();
    sensors.requestTemperatures();
}
float calib_factor= 1.0;
float getcalib(){
  calib_factor=(sensors.getUserDataByIndex(0)/100000.0)+1.0;
return calib_factor;
}
void setUserData(uint16_t data){
 sensors.setUserDataByIndex(0,data);
}
void printCalib(){
  Serial.print("stored calibData: ");
   Serial.println(getcalib(),6);
}
void initSampling(){
    for(int i =0; i < MAXDATA; i++){
      Data[i].temp = 0;
      Data[i].sampletime = 0;
      Data[i].dTemp = 0;
      Data[i].dTime = 0;
    }
    Data[0].temp = sensors.getTempCByIndex(0);
    Data[0].sampletime = millis();
    Data[0].dTemp = 0;
    Data[0].dTime = 0;
}
unsigned long start = 0;
unsigned long ending = 0;
unsigned long last ;

void PrintTemp(uint8_t index){
#ifdef DEBUG
   Serial.print("Index: ");
   Serial.print(index);
   Serial.print(", maxtemp: ");
   Serial.print(maxtemp,6);
     Serial.print(", starttemp: ");
   Serial.print(starttemp,6);
   Serial.print(", starttime: ");
      Serial.print(start,6);
   Serial.print(", Temp: ");
   Serial.print(Data[index].temp,6);
   Serial.print(", Sampletime: ");
   Serial.print(Data[index].sampletime);
   Serial.print(", d Temp: ");
   Serial.print(Data[index].dTemp,6);
   Serial.print(", d Time: ");
   Serial.println(Data[index].dTime);
#endif
   
}

void PrintManual(){
  Serial.println("");
  Serial.println("");
  Serial.println("          CO2 Laser Powermeter 20-100W");
  Serial.println("");
  Serial.println("Make sure the sensor is at ambient temperature");
  Serial.println("Fire the laser at the sensor block full power for 20 Seconds");
  Serial.println("");
  Serial.println("Ths program will detect the temperature rise and start the mesurement automatically.");
  Serial.println("");
  Serial.println("");
  Serial.println("Waiting for Beam");
  Serial.println("");
  Serial.println("");
  
  if(CALIBRATE){
    Serial.println("THIS IS COMPILED WITH CALIBRATE 1 AND WILL CHANGE CALIBRATION!");
    Serial.println("");
    Serial.println("");
  }
}



void printID(){
    Serial.println("");
    Serial.print("Sensor ID: ");
    DeviceAddress deviceAddress;
    sensors.getAddress(deviceAddress,0);
    for(int i = 0; i < 8; i++){
      // display.print(deviceAddress[i],HEX);
      Serial.print(deviceAddress[i],HEX);
    }
Serial.println("");
}


void setup() {
  // start serial port at 115200 Baud
  Serial.begin(115200);
  sensors.begin();
  sensors.requestTemperatures();
  //    delay(2000);
  printCalib();
  //Serial.print("User Data: ");
 // Serial.println(calib_factor,6);

   printID();
 // sensors.requestTemperatures();
  float temp = sensors.getTempCByIndex(0);
  Serial.print("Ambient Temperature: ");
   Serial.print(temp,6);
  initSampling();
  last = millis();
  PrintManual(); 
}

void detectbegin(){
  if(!start && Data[ringbufpointer].dTemp > 0.2){
    start = Data[prevof(prevof(ringbufpointer))].sampletime;
    starttemp =  Data[prevof(prevof(ringbufpointer))].temp;
    maxtemp = 0;
    Serial.println("");
    Serial.println("BEAM DETECTED");
  }
}

void detectEnd(){
  if(start){
    if((millis()-start) > 50000){
      Serial.print("Time to Long");
      while (true){
        
      }
      Serial.println(".");
    //Serial.println("BEAM DETECTED");
    }
    if(Data[ringbufpointer].dTemp <= 0.0){
    ending = Data[ringbufpointer].sampletime;
    printResult();
    start = 0;
    }
  }
}


void printResult(){
  Serial.println("");
  Serial.println("");
  Serial.println("Result: ");
  if(CALIBRATE){
    float measured = (GetWatts(maxtemp-starttemp, 20000));
    float cal = REFERENCE/measured;
   
    setUserData((int16_t)((cal-1.0)*100000));
    Serial.print("Saved calib factor: ");
    Serial.println(cal,8); 
    getcalib();
    Serial.print(((GetWatts(maxtemp-starttemp, 20000)+0.5)*calib_factor),4);
    Serial.println("W");
  }
  else{
    Serial.println("");
      Serial.print(((GetWatts(maxtemp-starttemp, 20000)+0.5)*calib_factor),0);
      Serial.println("W");
      Serial.println("");
      Serial.println("W");
      Serial.println("Let sensor cooldown for 15min and reset to restart measurement");
      //Serial.print((GetWatts(maxtemp-starttemp, 20000)+0.5),0);
      while(1){
        
      }
  }
}
void loop() {
  // put your main code here, to run repeatedly:
  unsigned long now = millis();
 if((now - last) > 999){   
  getTemp();
  if (DEBUG){
    PrintTemp(ringbufpointer);  
  }
  last = now;
  detectbegin();
  detectEnd();
 }
}

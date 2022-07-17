#ifndef HELPERS_H_
#define HELPERS_H_
#include "W_Struct.h"
// Required for I2C Sensors
#include "SparkFunBME280.h"
#include <LTR390.h>
LTR390 ltr390(LTR_ADDRESS);

// BME setup
BME280 bme;
byte temp = 0;
byte humidity = 0;
unsigned int pressure = 0;

void sensor_config() {
  Wire.begin();
  if (!adc.init()) {
  #ifdef DEBUG
      Serial.println("ADS1115 not connected!");
  #endif
  }
  adc.setVoltageRange_mV(ADS1115_RANGE_4096);
  bme.setI2CAddress(0x76); //Connect to a second sensor
  bme.beginI2C();
  bme.setMode(MODE_SLEEP);
  if (bme.beginI2C() == false) Serial.println("bme connect failed");
  if(!ltr390.init()){
    Serial.println("LTR390 not connected!");
  }
  ltr390.setMode(LTR390_MODE_ALS);
  ltr390.setGain(LTR390_GAIN_3);
  ltr390.setResolution(LTR390_RESOLUTION_18BIT);
}

void I2C_read() {
  bme.setMode(MODE_FORCED);
  while (bme.isMeasuring() == false) ; //Wait for sensor to start measurment
  while (bme.isMeasuring() == true) ; //Hang out while sensor completes the reading
  temp = bme.readTempF();
  pressure = (bme.readFloatPressure() / 10) + 0.5;
  Serial.println(bme.readFloatPressure());
  Serial.println(pressure);
  humidity = bme.readFloatHumidity() + 0.5; //0.5 rounds properly
  bme.setMode(MODE_SLEEP);
//  if (ltr390.newDataAvailable()) {
//      if (ltr390.getMode() == LTR390_MODE_ALS) {
//         Serial.print("Ambient Light Lux: "); 
//         Serial.println(ltr390.getLux());
//         ltr390.setGain(LTR390_GAIN_18);                  //Recommended for UVI - x18
//         ltr390.setResolution(LTR390_RESOLUTION_20BIT);   //Recommended for UVI - 20-bit
//         ltr390.setMode(LTR390_MODE_UVS);             
//      } else if (ltr390.getMode() == LTR390_MODE_UVS) {
//         Serial.print("UV Index: "); 
//         Serial.println(ltr390.getUVI());
//         ltr390.setGain(LTR390_GAIN_3);                   //Recommended for Lux - x3
//         ltr390.setResolution(LTR390_RESOLUTION_18BIT);   //Recommended for Lux - 18-bit
//         ltr390.setMode(LTR390_MODE_ALS);
//      }
//  }
//  return ws;
}


w_data load_data() {
  w_data ws;
  strcpy(ws.Time, rtc.getTime("%FT%T").c_str());
  ws.temp = temp;
  ws.humidity = humidity;
  ws.pressure = pressure;
  ws.wdir = Wind.wdir;
  ws.gust = Wind.gust;
  ws.wspd = Wind.wspd;
  ws.r_hour_sum = Rain.r_hour_sum;
  ws.r_24_sum = Rain.r_24_sum;
  ws.r_mid = Rain.r_mid;
  ws.soil_moisture = Rain.soil_moisture;
  ws.batteryLevel = batteryLevel;
  ws.dutyCycle = dutyCycle;
  ws.solar_mV = solar_mV;
  ws.bat_mV = bat_mV;
  switch(charge_mode_out) {
    case 0 :
      ws.charge_mode = "OFF";
      break;
    case 1 :
      ws.charge_mode = "Bulk charge";
      break;
    case 2 :
      ws.charge_mode = "Absorption charge";
      break;
    case 3 :
      ws.charge_mode = "Float charge";
      break;
   }
  return ws;
}


#endif

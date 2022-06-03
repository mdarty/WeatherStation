//#define DEBUG
//#define SEND_APRS

//Libraries
#include <WiFi.h>
#include "driver/adc.h"

#include "esp_system.h" //This inclusion configures the peripherals in the ESP system.

//NTP
#include <ESP32Time.h>

// Required for I2C Sensors
#include <ADS1115_WE.h> 
#include <Wire.h>
#include "SparkFunBME280.h"

#include <ArduinoJson.h>
#include <PubSubClient.h>

//My stuff
#include "settings.h"
#include "MQTT.h"
#include "APRS.h"
#include "Sleep.h"

ADS1115_WE adc = ADS1115_WE(I2C_ADDRESS);

#include "Rain.h"
#include "Wind.h"

//Variables
bool collect_dots = false;
byte batteryLevel = 0;

//timers
volatile unsigned long five_seconds = millis();
volatile unsigned long five_minutes = millis();
volatile unsigned long ntp_timer = millis();

ESP32Time rtc;
struct tm timeinfo;
bool ModemSleep = true;

// BME setup
BME280 bme;
byte temp = 0;
byte humidity = 0;
unsigned int pressure = 0;

float readChannel(ADS1115_MUX channel) {
  float voltage = 0.0;
  adc.setCompareChannels(channel);
  adc.startSingleMeasurement();
  while(adc.isBusy()){}
  voltage = adc.getResult_mV(); // alternative: getResult_mV for Millivolt
  return voltage;
}
    

void I2C_read() {
    bme.setMode(MODE_FORCED);
    while(bme.isMeasuring() == false) ; //Wait for sensor to start measurment
    while(bme.isMeasuring() == true) ; //Hang out while sensor completes the reading    
    temp = bme.readTempF();
    pressure = (bme.readFloatPressure()/10) + 0.5;
    Serial.println(bme.readFloatPressure());
    Serial.println(pressure);
    humidity = bme.readFloatHumidity() + 0.5; //0.5 rounds properly
    bme.setMode(MODE_SLEEP);
    batteryLevel = map(readChannel(bat_pin), bat_emp, bat_full, 0, 100);
}

WindMath Wind;
RainMath Rain;

w_data load_data() {
  w_data ws;
  strcpy(ws.Time, rtc.getTime("%FT%T").c_str());
  ws.temp = temp;
  ws.humidity = humidity;
  ws.pressure = pressure;
  ws.pressure = Wind.wdir;
  ws.gust = Wind.gust;
  ws.wspd = Wind.wspd;
  ws.r_hour_sum = Rain.r_hour_sum;
  ws.r_24_sum = Rain.r_24_sum;
  ws.r_mid = Rain.r_mid;
  ws.soil_moisture = Rain.soil_moisture; 
  ws.batteryLevel = batteryLevel;
  return ws;
}


void setup() {
  // put your setup code here, to run once: 
  #ifdef DEBUG
    Serial.begin(115200);
    Serial.println("Setup");
  #endif
  Wire.begin();
  if(!adc.init()){
    #ifdef DEBUG
    Serial.println("ADS1115 not connected!");
    #endif
  }
  adc.setVoltageRange_mV(ADS1115_RANGE_4096);
  bme.setI2CAddress(0x76); //Connect to a second sensor
  bme.beginI2C();
  bme.setMode(MODE_SLEEP);
  if(bme.beginI2C() == false) Serial.println("bme connect failed");
  wifi_connect();
  /*---------set with NTP---------------*/
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  if (getLocalTime(&timeinfo)){
    rtc.setTimeStruct(timeinfo); 
  }
  
  pinMode(RAIN_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(RAIN_PIN), rain_inc, FALLING);
  pinMode(WSPD_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(WSPD_PIN), wspd_inc, FALLING);
  //setModemSleep();//"AB1CDE-10>APRS,AB1CDE:=1234.12N/12345.12E-QTH von AB1CDE"
  #ifdef MQTT
    mqtt_client.setBufferSize(mqtt_buf);
    mqtt_client.setKeepAlive(report_time+1000);
    MQTT_config();
  #endif
  #ifdef APRS
    APRS_send(); //FW0690>APRS,TCPIP*:!DDMM.hhN/DDDMM.hhW$comments
  #endif
}

void loop() {
  if (millis() - five_seconds > sample_time){
    //Collect wind sample every 5 seconds, this is required for gusts
    if (collect_dots) {
      Serial.print(".");
    } else{
      Serial.print("Collect Wind Sample ");
      collect_dots=true;
    }
    Serial.print(".");
    
    Wind.collect_sample();
    five_seconds = millis();
  }else if(millis() - five_minutes > report_time){
    //Read everything and calculated needed things
    collect_dots = false;
    Serial.println(" ");
    Serial.println("I2C Samples");
    I2C_read();
    Serial.print("Rain Samples");
    Rain.calc(rtc.getMinute(), rtc.getHour(), rtc.getMonth(), rtc.getDay(), rtc.getDayofWeek());
    Serial.println(" ");
    Serial.println("Wind Samples");
    Wind.calc();
    Serial.println("Done Calc Samples");
    five_minutes = millis();
    dataReady = true;
  }else if( dataReady && (millis() - aprs_timer > report_time) && (millis() - five_delay > 5000) ){
    enableWiFi();
    if (millis() - ntp_timer > ntp_time) {
      if (getLocalTime(&timeinfo)) {
        rtc.setTimeStruct(timeinfo);
      }
    }
    collect_dots = false;
    Serial.println(" ");
    Serial.println("Report Sample");
    w_data ws = load_data();
    #ifdef MQTT
      MQTT_send_data(ws);
    #endif
    #ifdef APRS
      APRS_send(ws);
      aprs_timer = millis();
    #endif
    dataReady = false;
    disableWiFi();
  }
}

#include "settings.h"

//Libraries
#include <WiFi.h>
#include "driver/adc.h"
#include "esp_system.h" //This inclusion configures the peripherals in the ESP system.

//NTP
#include <ESP32Time.h>

//Variables
bool dataReady = false;
bool collect_dots = false;
byte batteryLevel = 0;

ESP32Time rtc;
struct tm timeinfo;
bool ModemSleep = true;

//timers
volatile unsigned long five_seconds = millis();
volatile unsigned long five_minutes = millis();
volatile unsigned long ntp_timer = millis();
volatile unsigned long aprs_timer = millis();
volatile unsigned long five_delay = millis();
volatile unsigned long bat_delay = millis();

#include <ADS1115_WE.h>
#include <Wire.h>
ADS1115_WE adc = ADS1115_WE(ADC_ADDRESS);

//My stuff
#include "Rain.h"
#include "Wind.h"
WindMath Wind;
RainMath Rain;
#include "Battery.h"

#include "helpers.h"

#ifdef MQTT
#include "MQTT.h"
#endif

#ifdef APRS
#include "APRS.h"
#endif

#include "Sleep.h"

void setup() {
  // put your setup code here, to run once:
  #ifdef DEBUG
    Serial.begin(115200);
    Serial.println("Setup");
  #endif
  sensor_config();
  wifi_connect();
  /*---------set with NTP---------------*/
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  if (getLocalTime(&timeinfo)) {
    rtc.setTimeStruct(timeinfo);
  }
  rain_setup();
  wind_setup();
  //setModemSleep();//"AB1CDE-10>APRS,AB1CDE:=1234.12N/12345.12E-QTH von AB1CDE"
  #ifdef MQTT
    MQTT_config();
  #endif
  #ifdef APRS
    APRS_send(); //FW0690>APRS,TCPIP*:!DDMM.hhN/DDDMM.hhW$comments
  #endif
  bat_setup();
  bat_charge();
}

void loop() {
  if (millis() - five_seconds > sample_time) {
    //Collect wind sample every 5 seconds, this is required for gusts
    if (collect_dots) {
      Serial.print(".");
    } else {
      Serial.print("Collect Wind Sample ");
      collect_dots = true;
    }
    Serial.print(".");

    Wind.collect_sample();
    five_seconds = millis();
  } else if (millis() - five_minutes > report_time) {
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
  } else if ( dataReady && (millis() - aprs_timer > report_time) && (millis() - five_delay > 5000) ) {
#ifdef SLEEP
    enableWiFi();
#endif
    if (millis() - ntp_timer > ntp_time) {
      if (getLocalTime(&timeinfo)) {
        //record time difference?
        rtc.setTimeStruct(timeinfo);
      }
      #ifdef MQTT
        MQTT_config();
      #endif
    }
    collect_dots = false;
    Serial.println(" ");
    Serial.println("Report Sample");
    w_data ws = load_data();
    aprs_timer = millis();
    dataReady = false;
#ifdef MQTT
    MQTT_send_data(ws);
#endif
#ifdef APRS
    APRS_send_data(ws);
#endif
#ifdef SLEEP
    disableWiFi();
#endif
  } else if (millis() - bat_delay > 5000) {
    bat_delay = millis();
    bat_charge();
  }
}

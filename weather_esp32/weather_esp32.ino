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

// BME setup
BME280 bme;
byte temp = 0;
byte humidity = 0;
unsigned int pressure = 0;

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

//My stuff
#include "settings.h"

ADS1115_WE adc = ADS1115_WE(I2C_ADDRESS);

#include "Battery.h"

#ifdef MQTT
#include "MQTT.h"
#endif

#ifdef APRS
#include "APRS.h"
#endif

#ifdef SLEEP
#include "Sleep.h"
#endif


#include "Rain.h"
#include "Wind.h"

WindMath Wind;
RainMath Rain;

#include "helpers.h"

void setup() {
  // put your setup code here, to run once:
#ifdef DEBUG
  Serial.begin(115200);
  Serial.println("Setup");
#endif
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
  wifi_connect();
  /*---------set with NTP---------------*/
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  if (getLocalTime(&timeinfo)) {
    rtc.setTimeStruct(timeinfo);
  }

  pinMode(RAIN_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(RAIN_PIN), rain_inc, FALLING);
  pinMode(WSPD_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(WSPD_PIN), wspd_inc, FALLING);
  //setModemSleep();//"AB1CDE-10>APRS,AB1CDE:=1234.12N/12345.12E-QTH von AB1CDE"
#ifdef MQTT_H_
  mqtt_client.setBufferSize(mqtt_buf);
  mqtt_client.setKeepAlive(report_time + 1000);
  MQTT_config();
#endif
#ifdef APRS_H_
  APRS_send(); //FW0690>APRS,TCPIP*:!DDMM.hhN/DDDMM.hhW$comments
#endif
  bat_setup();
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
#ifdef SLEEP_H_
    enableWiFi();
#endif
    if (millis() - ntp_timer > ntp_time) {
      if (getLocalTime(&timeinfo)) {
        //record time difference?
        rtc.setTimeStruct(timeinfo);
      }
      MQTT_config();
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
#ifdef SLEEP_H_
    disableWiFi();
#endif
  } else if (millis() - bat_delay > 5000) {
    bat_delay = millis();
    bat_charge();
  }
}

#define DEBUG
#ifdef DEBUG
  bool collect_dots = false;
#endif

#include "settings.h"
#include "esp_system.h" //This inclusion configures the peripherals in the ESP system.

#include <WiFi.h>
#include "driver/adc.h"

#include <BetterOTA.h>

//APRS
#include <APRS-IS.h>
String aprs_msg;
bool aprs_connected = false;
bool dataReady = false;

//Battery
byte batteryLevel = 0;

//NTP
#include <ESP32Time.h>

//MQTT
#include <PubSubClient.h>
#include <ArduinoJson.h>
WiFiClient espClient;

// Required for I2C Sensors
#include <Wire.h>
#include "SparkFunBME280.h"

#include "format_text.h"
#include "Rain.h"
#include "Wind.h"

//timers
volatile unsigned long five_seconds = millis();
volatile unsigned long five_minutes = millis();
volatile unsigned long aprs_timer = millis();
volatile unsigned long mqtt_timer = millis();
volatile unsigned long five_delay = millis();

ESP32Time rtc;
APRS_IS aprs_is(APRS_USER, APRS_PASS, APRS_TOOL, APRS_VERS);
bool ModemSleep = true;

// BME setup
BME280 bme;
byte temp = 0;
byte humidity = 0;
unsigned int pressure = 0;

void I2C_read() {
    bme.setMode(MODE_FORCED);
//      long startTime = millis();
    while(bme.isMeasuring() == false) ; //Wait for sensor to start measurment
    while(bme.isMeasuring() == true) ; //Hang out while sensor completes the reading    
//  long endTime = millis();
    temp = bme.readTempF();
    pressure = (bme.readFloatPressure()/10) + 0.5;
    Serial.println(bme.readFloatPressure());
    Serial.println(pressure);
    humidity = bme.readFloatHumidity() + 0.5; //0.5 rounds properly
    bme.setMode(MODE_SLEEP);
}

WindMath Wind;
RainMath Rain;

void wifi_connect(){
  #ifdef DEBUG
  Serial.printf("Connecting to %s ", WIFI_NAME);
  #endif
  WiFi.begin(WIFI_NAME, WIFI_KEY);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      #ifdef DEBUG
      Serial.print(".");
      #endif
  }
  #ifdef DEBUG
  Serial.println(" CONNECTED");
  #endif
}

//https://www.mischianti.org/2021/03/10/esp32-power-saving-modem-and-light-sleep-2/
//void disableWiFi(){
//    adc_power_off();
//    WiFi.disconnect(true);  // Disconnect from the network
//    WiFi.mode(WIFI_OFF);    // Switch WiFi off
//    Serial.println("");
//    Serial.println("WiFi disconnected!");
//}
//
//void enableWiFi(){
//    adc_power_on();
//    delay(200);
// 
//    WiFi.disconnect(false);  // Reconnect the network
//    WiFi.mode(WIFI_STA);    // Switch WiFi off
// 
//    delay(200);
// 
//    wifi_connect();
//}
 
void wakeModemSleep() {
    #ifdef DEBUG
      Serial.println("Waking Wifi");
    #endif
    setCpuFrequencyMhz(240);
    //enableWiFi();
    ModemSleep = false;
}

void setModemSleep() {
    #ifdef DEBUG
      Serial.println("Sleeping Wifi");
    #endif
    WiFi.setSleep(true);
    #ifdef DEBUG
    if (!setCpuFrequencyMhz(40)){
        Serial.println("Not valid frequency!");
    }
    #else
    setCpuFrequencyMhz(40);
    #endif
    // Use this if 40Mhz is not supported
    // setCpuFrequencyMhz(80);
    ModemSleep = true;
}

//void setModemSleep() {
//    disableWiFi();
//    setCpuFrequencyMhz(40);
//    // Use this if 40Mhz is not supported
//    // setCpuFrequencyMhz(80);
//}


void callback(char* topic, byte* payload, unsigned int length) {
  // handle message arrived
  char incoming_msg[150];
  char msg[100];

  #ifdef DEBUG
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
  #else
    OTATerminal.print("Message arrived [");
    OTATerminal.print(topic);
    OTATerminal.print("] ");
  #endif

  for (int i = 0; i < length; i++)
  {
    #ifdef DEBUG
      Serial.print((char)payload[i]);
    #else
      OTATerminal.print((char)payload[i]);
    #endif
    incoming_msg[i] = (char)payload[i];
  }
  incoming_msg[length] = 0;
  Serial.println(" ");
}

PubSubClient mqtt_client(mqtt_server, mqtt_port, callback, espClient);


void MQTT_config_per(String dev, String name, String unit, String id, String topic) {
  StaticJsonDocument<300> doc;
  doc["device_class"] = dev;
  doc["name"] = name;
  doc["state_topic"] = mqtt_topic;
  doc["unit_of_measurment"] = unit;
  doc["value_template"] = "{{value_json."+ id + "}}";
  char JSONmessageBuffer[300];
  serializeJson(doc, JSONmessageBuffer);
  String s = mqtt_topic + topic + "/config";
  if (mqtt_client.publish(s.c_str(), JSONmessageBuffer) == true) {
    #ifdef DEBUG
      Serial.println("Success sending message");
    #else
      OTATerminal.println("Success sending message");
    #endif
  } else {
    #ifdef DEBUG
      Serial.println("Error sending message");
    #else
      OTATerminal.println("Error sending message");
    #endif
  }
}

void MQTT_config() {
  MQTT_config_per("tempature", "Outdoor Tempature", "F", "Tempature", "sensorWeatherT");
  MQTT_config_per("humidity", "Outdoor Humidity", "%", "Humidity", "sensorWeatherH");
  MQTT_config_per("Pressure", "Outdoor Pressure", "hPa", "Pressure", "sensorWeatherP");
  
  MQTT_config_per("Wind Direction", "Wind Direction", "deg", "Wind Direction", "sensorWeatherWD");
  MQTT_config_per("Wind Speed", "Wind Speed", "mph", "Wind Speed", "sensorWeatherWS");
  MQTT_config_per("Wind Gust", "Wind Gust", "mph", "Wind Gust", "sensorWeatherWG");

  MQTT_config_per("Rain Last hr", "Rain Last hr", "mph", "Rain Last hr", "sensorWeatherR");
  MQTT_config_per("Rain Since Midnight", "Rain Since Midnight", "mph", "Rain Since Midnight", "sensorWeatherRM");
  MQTT_config_per("Rain Last 24 hrs", "Rain Last 24 hours", "mph", "Rain Last 24 hrs", "sensorWeatherRD");

  MQTT_config_per("Battery", "Weather Battery", "%", "Battery", "sensorWeatherB");
  mqtt_client.loop();
}

void MQTT_send() {
     //strcat(mqtt_topic, 'Tempature', true (retain))
    //https://techtutorialsx.com/2017/04/29/esp32-sending-json-messages-over-mqtt/
    StaticJsonDocument<300> doc;
   
    doc["device"] = "Weather Station";
    doc["Time"] = rtc.getTimeDate();
    doc["Tempature"] = temp;
    doc["Humidity"] = humidity;
    doc["Pressure"] = pressure;
    doc["Wind Direction"] = Wind.wdir;
    doc["Wind Speed"] = Wind.wspd;
    doc["Wind Gust"] = Wind.gust;
    doc["Rain Last hr"] = Rain.r_hour_sum;
    doc["Rain Since Midnight"] = Rain.r_mid;
    doc["Rain Last 24 hrs"] = Rain.r_24_sum;
    doc["Battery"] = batteryLevel;
    char JSONmessageBuffer[300];
    serializeJson(doc, JSONmessageBuffer);
    #ifdef DEBUG
      Serial.println("Sending message to MQTT topic..");
      Serial.println(JSONmessageBuffer);
    #else
      OTATerminal.println("Sending message to MQTT topic..");
      OTATerminal.println(JSONmessageBuffer);
    #endif

    String s = mqtt_topic + "state";
    if (mqtt_client.publish(s.c_str(), JSONmessageBuffer) == true) {
      #ifdef DEBUG
        Serial.println("Success sending message");
      #else
        OTATerminal.println("Success sending message");
      #endif
    } else {
      #ifdef DEBUG
        Serial.println("Error sending message");
      #else
        OTATerminal.println("Error sending message");
      #endif
    }   
    mqtt_client.loop();
}

void APRS_send(String aprs_msg) {
  if(!aprs_is.connected()){
    aprs_connected = false;
    if(!aprs_is.connect(APRS_SERVER, APRS_PORT, APRS_FILTER)){
        //If connection fails do otherstuff for 5 seconds then try again.
        #ifdef DEBUG
          Serial.println("APRS Connection Failed");
        #endif
        five_delay = millis();
      } else {aprs_connected = true;}
  } else {aprs_connected = true;}
  if (aprs_connected) {
    //FW0690>APRS,TCPIP*:@152457h4427.67N/02608.03E_.../...g...t044h86b10201L001WxUno
  
    #ifdef DEBUG
      Serial.println(aprs_msg);
      aprs_is.sendMessage(aprs_msg);
    #else
      aprs_is.sendMessage(aprs_msg);
    #endif
    aprs_timer = millis();
    dataReady = false;
    //setModemSleep();
    //Sleep wifi we shouldn't need it for 5 minutes
  }
}

void setup() {
  // put your setup code here, to run once: 
  #ifdef DEBUG
    Serial.begin(115200);
    Serial.println("Setup");
  #endif
  Wire.begin();
  Wire.setClock(400000);
  bme.setI2CAddress(0x76); //Connect to a second sensor
  bme.beginI2C();
  bme.setMode(MODE_SLEEP);
  if(bme.beginI2C() == false) Serial.println("bme connect failed");
  wifi_connect();
  OTACodeUploader.begin();
  OTATerminal.begin();
  /*---------set with NTP---------------*/
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  struct tm timeinfo;
  if (getLocalTime(&timeinfo)){
    rtc.setTimeStruct(timeinfo); 
  }
  
  pinMode(RAIN_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(RAIN_PIN), rain_inc, FALLING);
  pinMode(WSPD_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(WSPD_PIN), wspd_inc, FALLING);
  //setModemSleep();//"AB1CDE-10>APRS,AB1CDE:=1234.12N/12345.12E-QTH von AB1CDE"
  APRS_send(String(APRS_USER) + APRS_SSID + APRS_HEADER + "=" + aprsLocation + "_"); // + "-QTH von KF5RHG"); //FW0690>APRS,TCPIP*:!DDMM.hhN/DDDMM.hhW$comments
}

void loop() {
  // put your main code here, to run repeatedly:
  //  aprs_is();
  // else if to prioritize top executions.
  BetterOTA.handle();
  if (millis() - five_seconds > sample_time){
    //Collect wind sample every 5 seconds, this is required for gusts
    #ifdef DEBUG
      if (collect_dots) {
        Serial.print(".");
      } else{
        Serial.print("Collect Wind Sample ");
        collect_dots=true;
      }
    #else
      OTATerminal.println("Collect Wind Sample");
    #endif
    Wind.collect_sample();
    five_seconds = millis();
  }else if(millis() - five_minutes > report_time){
    //Read everything and calculated needed things
    #ifdef DEBUG
      collect_dots = false;
      Serial.println(" ");
      Serial.println("I2C Samples");
    #endif
    I2C_read();
    #ifdef DEBUG
      Serial.print("Rain Samples");
    #else
      OTATerminal.print("Rain Samples");
    #endif
    Rain.calc(rtc.getMinute(), rtc.getHour(), rtc.getMonth(), rtc.getDay(), rtc.getDayofWeek());
    #ifdef DEBUG
      Serial.println(" ");
      Serial.println("Wind Samples");
    #else
      OTATerminal.println(" ");
      OTATerminal.println("Wind Samples");
    #endif
    Wind.calc();
    #ifdef DEBUG
      Serial.println("Done Calc Samples");
    #else
      OTATerminal.println("Done Calc Samples");
    #endif
    batteryLevel = map(analogRead(bat_pin), 0.0f, 4095.0f, 0, 100);
    five_minutes = millis();
    dataReady = true;
  }else if( dataReady && (millis() - aprs_timer > report_time) && (millis() - five_delay > 5000) ){
    //Turn on wifi and publish all the things.
    #ifdef DEBUG
      collect_dots = false;
      Serial.println(" ");
      Serial.println("Report Sample");
    #endif
//    if (ModemSleep) {
//      //wake wifi if asleep
//      wakeModemSleep();
//    }
    if (!mqtt_client.connected()) {
//        mqttClient.connect(DEVICE_TOPIC, MQTT_USER, MQTT_PASS, MQTTavltopic, 0, true, "offline");
        #ifdef DEBUG
          Serial.println("Connecting to MQTT");
        #else
          OTATerminal.println("Connecting to MQTT");
        #endif
        mqtt_client.connect(mqtt_clientID, mqtt_user, mqtt_pass);
    }
    if (mqtt_client.connected()) {
      MQTT_send();
    }else{
      #ifdef DEBUG
        Serial.println("MQTT Failed to connect");
      #else
        OTATerminal.println("MQTT Failed to connect");
      #endif
    }
    String aprs_msg = String(APRS_USER) + APRS_SSID + APRS_HEADER + "@" + aprsLocation +
           "_" + Wind.getAPRS() + "t" + format(temp) + Rain.getAPRS() + "h" + format(humidity) +
           "b" + format_pressure(pressure);
               
    #ifdef DEBUG
      Serial.println("APRS");
      Serial.println(aprs_msg);
    #else
      OTATerminal.println("APRS");
      OTATerminal.println(aprs_msg);
      APRS_send(aprs_msg);
    #endif
  }
}

//#define DEBUG
#define post OTATerminal
//#define post Serial
//#define SEND_APRS
bool collect_dots = false;

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
//#include <StreamUtils.h>
WiFiClient espClient;
const int mqtt_buf = 528;

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
    while(bme.isMeasuring() == false) ; //Wait for sensor to start measurment
    while(bme.isMeasuring() == true) ; //Hang out while sensor completes the reading    
    temp = bme.readTempF();
    pressure = (bme.readFloatPressure()/10) + 0.5;
    post.println(bme.readFloatPressure());
    post.println(pressure);
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

void callback(char* topic, byte* payload, unsigned int length) {
  // handle message arrived
  char incoming_msg[150];
  char msg[100];

  post.print("Message arrived [");
  post.print(topic);
  post.print("] ");

  for (int i = 0; i < length; i++)
  {
    post.print((char)payload[i]);
    incoming_msg[i] = (char)payload[i];
  }
  incoming_msg[length] = 0;
  post.println(" ");
}

PubSubClient mqtt_client(mqtt_server, mqtt_port, callback, espClient);

void MQTT_send(const char *topic, JsonDocument *doc) {
    if (!mqtt_client.connected()) {
        post.print("Connecting to MQTT: ");
        mqtt_client.connect(mqtt_clientID, mqtt_user, mqtt_pass);
        byte i=0;
        while (!mqtt_client.connected()) {
          i++;
          post.print(".");
          delay(1);
          if (i > 200) {
            post.println(" ");
            break;
          }
        }
    }
    if (mqtt_client.connected()) {
      post.println("Connected");
      post.print("Attempting to publish topic: ");
      post.println(topic);
      post.println("Sending message to MQTT topic..");
      
      char JSONmessageBuffer[mqtt_buf];
      size_t n = serializeJson(*doc, JSONmessageBuffer);
      post.print("Size: ");
      post.println(n);
      if (mqtt_client.publish(topic, JSONmessageBuffer, n)) {
//      if (mqtt_client.publish(topic, JSONmessageBuffer, n) == true) {
        post.println("Success sending message");
      } else {
        post.println("Error sending message");
      }
    }else{
      post.println("MQTT Failed to connect");
    }
}

void MQTT_config_per(String dev, String name, String unit, String id, String topic) {
  StaticJsonDocument<mqtt_buf> doc;
  doc["device_class"] = dev;
  doc["name"] = name;
  doc["state_topic"] = mqtt_topic;
  doc["unit_of_measurment"] = unit;
  doc["value_template"] = "{{value_json."+ id + "}}";
  //doc["value"] = mqtt_topic + "data";
  String s = mqtt_topic + topic + "/config";
  MQTT_send(s.c_str(), &doc);
}

void MQTT_config() {
  MQTT_config_per("Time", "Weather Station Time", " ", "Time", "sensorWSTime");
  MQTT_config_per("Temperature", "Outdoor Temperature", "F", "Temp", "sensorWeatherT");
  MQTT_config_per("Humidity", "Outdoor Humidity", "%", "RH", "sensorWeatherH");
  MQTT_config_per("Pressure", "Outdoor Pressure", "hPa", "Pres", "sensorWeatherP");
  
  MQTT_config_per("Wind_Direction", "Wind_Direction", "deg", "wDir", "sensorWeatherWD");
  MQTT_config_per("Wind_Speed", "Wind_Speed", "mph", "wSpd", "sensorWeatherWS");
  MQTT_config_per("Wind_Gust", "Wind_Gust", "mph", "wGust", "sensorWeatherWG");

  MQTT_config_per("Rain_Last_hr", "Rain_Last_hr", "in", "rLstHr", "sensorWeatherR");
  MQTT_config_per("Rain_Since_Midnight", "Rain_Since_Midnight", "in", "rMidngt", "sensorWeatherRM");
  MQTT_config_per("Rain_Last_24_hrs", "Rain_Last_24_hours", "in", "rLst24hrs", "sensorWeatherRD");
  MQTT_config_per("SoilMoisture", "SoilMoisture", "%", "SoilMoisture", "sensorWeatherSM");

  MQTT_config_per("Battery", "Weather_Battery", "%", "Battery", "sensorWeatherB");
  mqtt_client.loop();
}

void MQTT_send_data_post(String id, String msg){
  String s = mqtt_topic + id + "/data";
  if (mqtt_client.publish(s.c_str(), msg.c_str())) {
    post.println("Success sending message: " + id);
  } else {
    post.println("Error sending message: " + id);
  }
}

void MQTT_send_data() {
     //strcat(mqtt_topic, 'Tempature', true (retain))
    //https://techtutorialsx.com/2017/04/29/esp32-sending-json-messages-over-mqtt/
    //https://codeblog.jonskeet.uk/2011/04/05/of-memory-and-strings/
    StaticJsonDocument<mqtt_buf> doc;
   
    doc["device"] = "WeatherStation";
    doc["Time"] = rtc.getTime("%FT%T");
    
    doc["Temp"] = temp;
    doc["RH"] = humidity;
    doc["Pres"] = pressure;
    
    doc["wDir"] = Wind.wdir;
    doc["wSpd"] = Wind.wspd;
    doc["wGust"] = Wind.gust;
    
    doc["rLstHr"] = (float) Rain.r_hour_sum/100.0;
    doc["rMidngt"] = (float) Rain.r_mid/100.0;
    doc["rLst24hrs"] = (float) Rain.r_24_sum/100.0;
    doc["SoilMoisture"] = Rain.soil_moisture;
    
    doc["Battery"] = batteryLevel;

    String s = mqtt_topic + "data";
    MQTT_send(s.c_str(), &doc);
    
//      if (!mqtt_client.connected()) {
//        post.print("Connecting to MQTT: ");
//        mqtt_client.connect(mqtt_clientID, mqtt_user, mqtt_pass);
//        byte i=0;
//        while (!mqtt_client.connected()) {
//          i++;
//          post.print(".");
//          delay(1);
//          if (i > 200) {
//            post.println(" ");
//            break;
//          }
//        }
//    }
//    if (mqtt_client.connected()) {
//      post.println("Connected");
//      MQTT_send_data_post("Time", String(rtc.getEpoch()));
//      MQTT_send_data_post("Temp", String(temp));
//      MQTT_send_data_post("RH", String(humidity));
//      MQTT_send_data_post("Pres", String(pressure));
//      MQTT_send_data_post("wDir", String(Wind.wdir));
//      MQTT_send_data_post("wSpd", String(Wind.wspd));
//      MQTT_send_data_post("wGust", String(Wind.gust));
//      MQTT_send_data_post("rLstHr", String(Rain.r_hour_sum));
//      MQTT_send_data_post("rMidngt", String(Rain.r_mid));
//      MQTT_send_data_post("rLst24hrs", String(Rain.r_24_sum));
//      MQTT_send_data_post("Battery", String(batteryLevel));
//      mqtt_client.loop();
//    }else{
//      post.println("MQTT Failed to connect");
//    }
}


void APRS_send(String aprs_msg) {
  if(!aprs_is.connected()){
    aprs_connected = false;
    if(!aprs_is.connect(APRS_SERVER, APRS_PORT, APRS_FILTER)){
        //If connection fails do otherstuff for 5 seconds then try again.
        #ifdef DEBUG
          Serial.println("APRS Connection Failed");
        #else
          OTATerminal.println("APRS Connection Failed");
        #endif
        five_delay = millis();
      } else {aprs_connected = true;}
  } else {aprs_connected = true;}
  if (aprs_connected) {
    //FW0690>APRS,TCPIP*:@152457h4427.67N/02608.03E_.../...g...t044h86b10201L001WxUno
    post.println(aprs_msg);
    #ifdef SEND_APRS
      aprs_is.sendMessage(aprs_msg);
    #endif
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
  mqtt_client.setBufferSize(mqtt_buf);
  MQTT_config();
  APRS_send(String(APRS_USER) + APRS_SSID + APRS_HEADER + "=" + aprsLocation + "_"); // + "-QTH von KF5RHG"); //FW0690>APRS,TCPIP*:!DDMM.hhN/DDDMM.hhW$comments
}

void loop() {
  // put your main code here, to run repeatedly:
  //  aprs_is();
  // else if to prioritize top executions.
  BetterOTA.handle();
  if (millis() - five_seconds > sample_time){
    //Collect wind sample every 5 seconds, this is required for gusts
    if (collect_dots) {
      post.print(".");
    } else{
      post.print("Collect Wind Sample ");
      collect_dots=true;
    }
    post.print(".");
    
    Wind.collect_sample();
    five_seconds = millis();
  }else if(millis() - five_minutes > report_time){
    //Read everything and calculated needed things
    collect_dots = false;
    post.println(" ");
    post.println("I2C Samples");
    I2C_read();
    post.print("Rain Samples");
    Rain.calc(rtc.getMinute(), rtc.getHour(), rtc.getMonth(), rtc.getDay(), rtc.getDayofWeek());
    post.println(" ");
    post.println("Wind Samples");
    Wind.calc();
    post.println("Done Calc Samples");
    batteryLevel = map(analogRead(bat_pin), 0.0f, 4095.0f, 0, 100);
    five_minutes = millis();
    dataReady = true;
  }else if( dataReady && (millis() - aprs_timer > report_time) && (millis() - five_delay > 5000) ){
    collect_dots = false;
    post.println(" ");
    post.println("Report Sample");
    MQTT_send_data();
    String aprs_msg = String(APRS_USER) + APRS_SSID + APRS_HEADER + "@" + aprsLocation +
     "_" + Wind.getAPRS() + "t" + format(temp) + Rain.getAPRS() + "h" + format(humidity) +
     "b" + format_pressure(pressure);
    APRS_send(aprs_msg);
    aprs_timer = millis();
    dataReady = false;
  }
}

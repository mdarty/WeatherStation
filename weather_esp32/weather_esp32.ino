//#define DEBUG
#define post OTATerminal
//#define post Serial
//#define SEND_APRS

//Libraries
#include <WiFi.h>
#include "driver/adc.h"

//MQTT
#include <PubSubClient.h>
#include <ArduinoJson.h>
//#include <StreamUtils.h>
WiFiClient espClient;


#include "esp_system.h" //This inclusion configures the peripherals in the ESP system.

#include <BetterOTA.h>

//APRS
#include <APRS-IS.h>

//NTP
#include <ESP32Time.h>

// Required for I2C Sensors
#include <ADS1115_WE.h> 
#include <Wire.h>
#include "SparkFunBME280.h"

//My stuff
#include "settings.h"
#include "format_text.h"

ADS1115_WE adc = ADS1115_WE(I2C_ADDRESS);

#include "Rain.h"
#include "Wind.h"

//Variables
bool collect_dots = false;

//APRS
String aprs_msg;
bool aprs_connected = false;
bool dataReady = false;

//Battery
byte batteryLevel = 0;

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
    post.println(bme.readFloatPressure());
    post.println(pressure);
    humidity = bme.readFloatHumidity() + 0.5; //0.5 rounds properly
    bme.setMode(MODE_SLEEP);
    batteryLevel = map(readChannel(bat_pin), bat_emp, bat_full, 0, 100);
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

void mqtt_callback(char* topic, byte* payload, unsigned int length) {
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

PubSubClient mqtt_client(mqtt_server, mqtt_port, mqtt_callback, espClient);

void MQTT_send(String topic, String JSONmessageBuffer) {
    if (!mqtt_client.connected()) {
        post.print("Connecting to MQTT: ");
        mqtt_client.connect(mqtt_clientID, mqtt_user, mqtt_pass, mqtt_LWT.c_str(), 0, false, mqtt_LWT_p);
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
      
//      char JSONmessageBuffer[mqtt_buf];
//      size_t n = serializeJson(doc, JSONmessageBuffer);
//      post.print("Size: ");
//      post.println(n);
      if (mqtt_client.publish(topic.c_str(), JSONmessageBuffer.c_str())) {
//      if (mqtt_client.publish(topic, JSONmessageBuffer, n) == true) {
        post.println("Success sending message");
      } else {
        post.println("Error sending message");
      }
    }else{
      post.println("MQTT Failed to connect");
    }
}

void MQTT_config() {
//  String topic;
//  StaticJsonDocument<mqtt_buf> doc;
  String out;
  String topic = "";
  DynamicJsonDocument doc(mqtt_buf);
  for (byte i=0; i < conf_count; i++) {
    out = mqtt_configs(i);
    deserializeJson(doc, out);
    topic = doc["config_topic"].as<String>();
    doc.remove("config_topic");
    out = "";
    serializeJson(doc, out);
    post.println(topic);
    post.println(out);
    MQTT_send(topic, out);
    doc.clear();
  }
  mqtt_client.loop();
}

//void MQTT_send_data_post(String id, String msg){
//  String s = mqtt_topic + id + "/data";
//  if (mqtt_client.publish(s.c_str(), msg.c_str())) {
//    post.println("Success sending message: " + id);
//  } else {
//    post.println("Error sending message: " + id);
//  }
//}

void MQTT_send_data() {
     //strcat(mqtt_topic, 'Tempature', true (retain))
    //https://techtutorialsx.com/2017/04/29/esp32-sending-json-messages-over-mqtt/
    //https://codeblog.jonskeet.uk/2011/04/05/of-memory-and-strings/
    DynamicJsonDocument doc(mqtt_buf);
   
    doc["device"] = mqtt_clientID;
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

//    char out[mqtt_buf];
    String out = "";
    serializeJson(doc, out);
    MQTT_send(mqtt_state_topic, out);
}

void APRS_send(String aprs_msg) {
  if(!aprs_is.connected()){
    aprs_connected = false;
    if(!aprs_is.connect(APRS_SERVER, APRS_PORT)){
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
//  Wire.setClock(400000);
  if(!adc.init()){
    Serial.println("ADS1115 not connected!");
  }
  adc.setVoltageRange_mV(ADS1115_RANGE_4096);
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
  mqtt_client.setKeepAlive(report_time+1000);
  MQTT_config();
  APRS_send(String(APRS_USER) + APRS_SSID + APRS_HEADER + "=" + aprsLocation + "_WeatherStation"); // + "-QTH von KF5RHG"); //FW0690>APRS,TCPIP*:!DDMM.hhN/DDDMM.hhW$comments
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

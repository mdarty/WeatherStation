#ifndef SETTINGS_H_
#define SETTINGS_H_

//Debouncing Time in MICROSECONDS (15 Milla seconds)
const int DEBOUNCING_TIME = 15000;

#ifdef DEBUG
  const int report_time = 30000;
#else 
  const int report_time = 300000;
#endif

const int sample_time = 5000;

const char* ntpServer = "SOME NTP SERVER";
const int daylightOffset_sec = 0;
const int gmtOffset_sec = 0;

// Wifi Info
const char* WIFI_NAME = "YOUR WIFI";
const char* WIFI_KEY = "YOUR WIFI PASS";

// Pins
#define I2C_ADDRESS 0x48
const byte RAIN_PIN = 33;
#define WDIR_PIN ADS1115_COMP_0_GND
const byte WSPD_PIN = 32;
//const byte bat_pin = 13;
#define bat_pin ADS1115_COMP_2_GND
const unsigned int bat_full = 2600; //mV scaled with resistor
const unsigned int bat_emp = 2000; //mV scaled with resistor
#define SOIL_PIN ADS1115_COMP_1_GND
const float Vref = 3291.625;


// Device name and software version
const char *NODENAME = "WxESP";
const char *VERSION = "0.1";
bool       PROBE  = true;                   // True if the station is being probed

const char* APRS_SERVER = "rotate.aprs2.net";
const int   APRS_PORT   = 14580;
const char* APRS_USER   = "YOUR CALLSIGN";
String APRS_SSID   = "-13";
String APRS_HEADER      = ">APRS,TCPIP*:";
const char* APRS_PASS   = "YOUR APRS PIN";
const char* APRS_TOOL   = "ESP32-WX";
const char* APRS_VERS   = "0.1";
String aprsLocation  = "latN/LongW";


// MQTT
const char* mqtt_server = "MQTT_SERVER";
const int mqtt_port = 1883;
const char* mqtt_clientID = "WeatherStation";
const char* mqtt_user = "MQTT_USER";
const char* mqtt_pass = "MQTT_PASS";
String hassio_topic = "homeassistant/sensor/weather/station/";
String mqtt_topic = "weather/station/sensor/";
String hassio_topic = "homeassistant/sensor/" + String(mqtt_clientID) + "/";
//String mqtt_topic = String(mqtt_clientID) + "/sensor/";
//const int mqtt_buf = 528;
const int mqtt_buf = 1024;
const char* mqtt_LWT_p = "Online";
String mqtt_LWT = hassio_topic + "LWT";
String mqtt_state_topic = hassio_topic + "state";

String mqtt_configs(byte i) {
  DynamicJsonDocument conf_doc(mqtt_buf);
  JsonObject conf_dev  = conf_doc.createNestedObject("dev");

  conf_dev["name"] = mqtt_clientID;
  conf_dev["mf"] = "Darty";
  conf_dev["sw"] = "0.1";
  conf_dev["ml"] = "Prototype";
  conf_dev["sa"] = "Outside";
  JsonArray ids = conf_dev.createNestedArray("ids");
  ids.add("Prototype");

  if (i == 0) {
    conf_doc["unique_id"] = String(mqtt_clientID) + "_temp";
    conf_doc["device_class"] = "temperature";
    conf_doc["name"] = "Outside Temperature";
    conf_doc["unit_of_meas"] = "°F";
    conf_doc["value_template"] = "{{ value_json.Temp }}";
  }
  
  if (i == 1) {  
    //Humidity
    conf_doc["unique_id"] = String(mqtt_clientID) + "_humidity";
    conf_doc["device_class"] = "humidity";
    conf_doc["name"] = "Outside Humidity";
    conf_doc["unit_of_meas"] = "%";
    conf_doc["value_template"] = "{{ value_json.RH }}";
  }

  if (i == 2) {
    //Pressure
    conf_doc["unique_id"] = String(mqtt_clientID) + "_pressure";
    conf_doc["device_class"] = "pressure";
    conf_doc["name"] = "Outdoor Temperature";
    conf_doc["unit_of_meas"] = "hPa";
    conf_doc["value_template"] = "{{ value_json.Pres }}";
  }

  if (i == 3) {
    //Wind Direction
    conf_doc["unique_id"] = String(mqtt_clientID) + "_wind_direction";
    conf_doc["name"] = "Wind Direction";
    conf_doc["icon"] = "mdi:compass";
    conf_doc["unit_of_meas"] = "deg";
    conf_doc["value_template"] = "{{ value_json.wDir }}";
  }

  if (i == 4) {
    // Wind Speed
    conf_doc["unique_id"] = String(mqtt_clientID) + "_wind_speed";
    conf_doc["name"] = "Wind Speed";
    conf_doc["icon"] = "mdi:weather-windy";
    conf_doc["unit_of_meas"] = "mph";
    conf_doc["value_template"] = "{{ value_json.wSpd }}";
   }

  if (i == 5) {
    // Wind Gust
    conf_doc["unique_id"] = String(mqtt_clientID) + "_wind_gust";
    conf_doc["name"] = "Wind Gust";
    conf_doc["icon"] = "mdi:weather-windy";
    conf_doc["unit_of_meas"] = "mph";
    conf_doc["value_template"] = "{{ value_json.wGust }}";
  }

  if (i == 6) {
    //Rain Last Hour
    conf_doc["unique_id"] = String(mqtt_clientID) + "_rain_last_hour";
    conf_doc["name"] = "Rain Last Hour";
    conf_doc["icon"] = "mdi:weather-rainy";
    conf_doc["unit_of_meas"] = "in";
    conf_doc["value_template"] = "{{ value_json.rLstHr }}";
  }

  if (i == 7) {
    //Rain Last 24 hours
    conf_doc["unique_id"] = String(mqtt_clientID) + "_rain_last_24hrs";
    conf_doc["name"] = "Rain Last 24 Hours";
    conf_doc["icon"] = "mdi:weather-rainy"; 
    //conf_doc["state_topic"] = mqtt_topic;
    conf_doc["unit_of_meas"] = "in";
    conf_doc["value_template"] = "{{ value_json.rLst24hrs }}";
  }

  if (i == 8) {
    //Rain Since Midnight
    conf_doc["unique_id"] = String(mqtt_clientID) + "_rain_since_midnight";
    conf_doc["name"] = "Rain Since Midnight";
    conf_doc["icon"] = "mdi:weather-rainy";
    conf_doc["unit_of_meas"] = "in";
    conf_doc["value_template"] = "{{ value_json.rMidngt }}";
  }

  if (i == 9) {
    //Soil Moisture
    conf_doc["unique_id"] = String(mqtt_clientID) + "_soil_moisture";
    conf_doc["name"] = "Soil Moisture";
    conf_doc["icon"] = "mdi:water-percent";
    conf_doc["unit_of_meas"] = "%";
    conf_doc["value_template"] = "{{ value_json.SoilMoisture }}";
  }

  if (i == 10) {
    //Battery
    conf_doc["unique_id"] = String(mqtt_clientID) + "_battery";
    conf_doc["device_class"] = "battery";
    conf_doc["name"] = "WS_Battery";
    conf_doc["unit_of_meas"] = "F";
    conf_doc["unit_of_meas"] = "%";
    conf_doc["value_template"] = "{{value_json.Battery}}";
  }

  if (i == 11) {
    //Time
    conf_doc["unique_id"] = String(mqtt_clientID) + "_timestamp";
    conf_doc["device_class"] = "timestamp";
    conf_doc["name"] = "WS Time"; 
    conf_doc["value_template"] = "{{value_json.Time}}";
  }

  conf_doc["avty_t"] = mqtt_LWT;
  conf_doc["pl_avail"] = "Online";
  conf_doc["pl_not_avail"] = "Offline";
  conf_doc["config_topic"] = hassio_topic + conf_doc["unique_id"].as<String>() + "/config";
  conf_doc["state_topic"] = mqtt_state_topic;
  String out = "";
  serializeJson(conf_doc, out);
  return out;
}

const int conf_count = 11;

#endif

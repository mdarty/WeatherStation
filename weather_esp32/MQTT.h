#ifndef MQTT_H_
#define MQTT_H_

#include <PubSubClient.h>
#include "W_Struct.h"

volatile unsigned long mqtt_timer = millis();

void mqtt_callback(char* topic, byte* payload, unsigned int length) {
  // handle message arrived
  char incoming_msg[150];
  char msg[100];
#ifdef DEBUG
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
#endif

  for (int i = 0; i < length; i++)
  {
    #ifdef DEBUG
      Serial.print((char)payload[i]);
    #endif
    incoming_msg[i] = (char)payload[i];
  }
  incoming_msg[length] = 0;
#ifdef DEBUG
  Serial.println(" ");
#endif
}

WiFiClient espClient;

PubSubClient mqtt_client(mqtt_server, mqtt_port, mqtt_callback, espClient);

void MQTT_setup() {
  
}

void MQTT_send(String topic, String JSONmessageBuffer) {
  if (!mqtt_client.connected()) {
    Serial.print("Connecting to MQTT: ");
    mqtt_client.connect(mqtt_clientID, mqtt_user, mqtt_pass, mqtt_LWT.c_str(), 0, true, mqtt_LWT_p);
    byte i = 0;
    while (!mqtt_client.connected()) {
#ifdef DEBUG
      i++;
      Serial.print(".");
      delay(1);
      if (i > 200) {
        Serial.println(" ");
        break;
      }
#endif
    }
  }
  if (mqtt_client.connected()) {
#ifdef DEBUG
    Serial.println("Connected");
    Serial.print("Attempting to publish topic: ");
    Serial.println(topic);
    Serial.println("Sending message to MQTT topic..");
#endif

    if (mqtt_client.publish(topic.c_str(), JSONmessageBuffer.c_str(), true)) {
      Serial.println("Success sending message");
    } else {
      Serial.println("Error sending message");
    }
  } else {
    Serial.println("MQTT Failed to connect");
  }
}

void MQTT_config() {
  //  String topic;
  //  StaticJsonDocument<mqtt_buf> doc;
  mqtt_client.setBufferSize(mqtt_buf);
  mqtt_client.setKeepAlive(report_time + 1000);
  String out;
  String topic = "";
  DynamicJsonDocument doc(mqtt_buf);
  for (byte i = 0; i < conf_count; i++) {
    out = mqtt_configs(i);
    deserializeJson(doc, out);
    topic = doc["config_topic"].as<String>();
    doc.remove("config_topic");
    out = "";
    serializeJson(doc, out);
    Serial.println(topic);
    Serial.println(out);
    MQTT_send(topic, out);
    doc.clear();
  }
  mqtt_client.loop();
}

void MQTT_send_data(w_data ws) {
  //strcat(mqtt_topic, 'Tempature', true (retain))
  //https://techtutorialsx.com/2017/04/29/esp32-sending-json-messages-over-mqtt/
  //https://codeblog.jonskeet.uk/2011/04/05/of-memory-and-strings/
  DynamicJsonDocument doc(mqtt_buf);

  doc["device"] = mqtt_clientID;
  doc["Time"] = String(ws.Time) + "+0000";

  doc["Temp"] = ws.temp;
  doc["RH"] = ws.humidity;
  doc["Pres"] = ws.pressure;

  doc["wDir"] = ws.wdir;
  doc["wSpd"] = (int)(ws.wspd/100.0 + 0.5)/10.0;
  doc["wGust"] = (int)(ws.gust/100.0 + 0.5)/10.0;

  doc["rLstHr"] = (float) ws.r_hour_sum / 100.0;
  doc["rMidngt"] = (float) ws.r_mid / 100.0;
  doc["rLst24hrs"] = (float) ws.r_24_sum / 100.0;
  doc["SoilMoisture"] = ws.soil_moisture;

  doc["Battery"] = ws.batteryLevel;
  doc["batMode"] = ws.charge_mode;
  doc["batDuty"] = ws.dutyCycle;
  doc["solarV"] = ws.solar_mV/1000.0;
  doc["batteryV"] = ws.bat_mV/1000.0;

  String out = "";
  serializeJson(doc, out);
  MQTT_send(mqtt_state_topic, out);
}


#endif

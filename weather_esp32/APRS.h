#ifndef APRS_H_
#define APRS_H_

//APRS
#include <APRS-IS.h>
#include "W_Struct.h"

String aprs_msg;
bool aprs_connected = false;

APRS_IS aprs_is(APRS_USER, APRS_PASS, APRS_TOOL, APRS_VERS);

String format(int input) {
  //APRS required 3 charater strings
  String s = String(input);
  byte l;
  if (input < 0) {
    l = 2;
  } else {
    l = 3;
  }
  while (s.length() < l) {
    s = "0" + s;
  }
  if (input < 0) {
    return "-" + s;
  } else {
    return s;
  }
}

String format_pressure(unsigned int input) {
  //APRS requires 5 charaters for pressure
  String s = String(input);
  while (s.length() < 5) {
    s = "0" + s;
  }
  return s;
}

void APRS_send_things(String aprs_msg) {
  if(!aprs_is.connected()){
    aprs_connected = false;
    if(!aprs_is.connect(APRS_SERVER, APRS_PORT)){
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
    #endif
    aprs_is.sendMessage(aprs_msg);
  }
}

void APRS_send(){
  APRS_send_things(String(APRS_USER) + APRS_SSID + APRS_HEADER + "=" + aprsLocation + "_WeatherStation"); // + "-QTH von KF5RHG"
}


//@171545z3306.05N/09727.49W_.../...g...t...r000p000P000b10155h...weewx-4.8.0-ObserverIP
void APRS_send_data(w_data ws){
    unsigned int iwdir = ws.wdir + 0.5;
    unsigned int iwspd = ws.wspd/1000.0 + 0.5;
    unsigned int igust = ws.gust/1000.0 + 0.5;
    String aprs_msg = String(APRS_USER) + APRS_SSID + APRS_HEADER
                    + "@" + ws.APRS_TIME + "z"
                    + aprsLocation + "_"
                    + format(iwdir) + "/" + format(iwspd) + "g" + format(igust)
                    + "t" + format(ws.temp)
                    + "r" + format(ws.r_hour_sum) + "p" + format(ws.r_24_sum) + "P" + format(ws.r_mid)
                    + "h" + format(ws.humidity)
                    + "b" + format_pressure(ws.pressure);
    APRS_send_things(aprs_msg);
}

#endif

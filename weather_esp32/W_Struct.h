#ifndef W_STRUCT_H_
#define W_STRUCT_H_
typedef struct w_data {
    char Time[20];
    char APRS_TIME[20];
    unsigned int temp;
    unsigned int humidity;
    unsigned int pressure;
    unsigned int wdir;
    unsigned int gust;
    unsigned int wspd;
    unsigned int r_hour_sum;
    unsigned int r_24_sum;
    unsigned int r_mid;
    unsigned int soil_moisture; 
    byte batteryLevel;
    String charge_mode;
    byte dutyCycle;
    unsigned int solar_mV;
    unsigned int bat_mV;
};
#endif

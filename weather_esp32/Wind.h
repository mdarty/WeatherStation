#ifndef WIND_H_
#define WIND_H_
#include <math.h>
unsigned long wspd_count = 0;
volatile unsigned long wspd_last_millis = millis();
volatile unsigned long wspd_last_micros = micros();
const unsigned int _wdir_len = report_time/sample_time + 0.5;

void wspd_inc() {
  if((long)(micros() - wspd_last_micros) >= DEBOUNCING_TIME) {
    wspd_count++;
    wspd_last_micros = micros();
  }
}

void wind_setup() {
  pinMode(WSPD_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(WSPD_PIN), wspd_inc, FALLING);
}

class WindMath {
  private:
    unsigned int _gust;
    unsigned int _wspd_count;
    unsigned int _wspd_sum;
    unsigned int _wspd;
    unsigned int _wdir_count;
    unsigned int _wdir_sum;
    unsigned int _wdir_index;
    float _wdir_array[_wdir_len];


    float readChannel(ADS1115_MUX channel) {
      float voltage = 0.0;
      adc.setCompareChannels(channel);
      adc.startSingleMeasurement();
      while(adc.isBusy()){}
      voltage = adc.getResult_mV(); // alternative: getResult_mV for Millivolt
      return voltage;
    }

  float _read_wind_dir() {
        //https://cdn.sparkfun.com/assets/d/1/e/0/6/DS-15901-Weather_Meter.pdf
        //10k resister
        float mV = readChannel(WDIR_PIN);
        unsigned int R = 10030 * mV / (Vref - mV) + 0.5;
        switch (R) {
          case 27441 ... 37559:   return 0;
          case 5236 ... 7384:   return 22.5;
          case 7386 ... 11159:   return 45;
          case 790 ... 945:   return 67.5;
          case 946 ... 1204:   return 90; //Bad
          case 587 ... 789:   return 112.5;
          case 1806 ... 2669:   return 135;
          case 1206 ... 1804:   return 157.5;
          case 3521 ... 5234:   return 180;
          case 2671 ... 3519:   return 202.5;
          case 15061 ... 18939:   return 225;
          case 11161 ... 15059:   return 247.5;
          case 92451 ... 147549:   return 270; //Bad
          case 37561 ... 53509:   return 292.5;
          case 53511 ... 92449:   return 315;
          case 18941 ... 27439:   return 337.5;
          default: return -1;
        }
    }

    
    int _wdir_next() {
      //circular index for 24 hours
      _wdir_index++;
      if (_wdir_index > _wdir_len) {
        _wdir_index = 0;
      }
      return _wdir_index;
    }


    int _wdir_average () {
      //can't just average due to circular statistics
      //https://projects.raspberrypi.org/en/projects/build-your-own-weather-station/7
      float r = 0.0;
      float sum_cos = 0.0;
      float sum_sin = 0.0;


      for (unsigned int i = 0; i < _wdir_len; i++) {
        r = _wdir_array[i] * DEG_TO_RAD;
        sum_cos += cos(r);
        sum_sin += sin(r);
      }
      
      float c = sum_cos/_wdir_len;
      float s = sum_sin/_wdir_len;
      int arc = RAD_TO_DEG * atan(s/c);

      int avg = 0;
      
      if ((s > 0) && (c > 0)) {
        avg = arc;
      } else if (c < 0) {
        avg = arc + 180;
      } else if ((s < 0) && (c > 0)) {
        avg = arc + 360;
      }

      if (avg == 360) {
        return 0;
      } else {
        return avg;
      }
    }
  
  public:
    unsigned int wdir;
    unsigned int gust;
    unsigned int wspd;

    void collect_sample() {
      // Speed 2.4km/h (1.5mph) is 1 count per second
      // 2.4km/hr
      // 1.49129 mph * 1,000,000 macroseconds
      // 1.49129 mph * 1000 milliseconds
      // micros unsigned long can't handle 5 seconds plus I only need 3 sigfigs.
      // https://cdn.sparkfun.com/assets/d/1/e/0/6/DS-15901-Weather_Meter.pdf
      _wspd = 0.5 + ((1491290 * wspd_count) / (millis() - wspd_last_millis)); //0.5 rounds properly.

        wspd_last_millis = 0;
      if (_gust < _wspd){
        _gust = _wspd;
      }
      _wspd_sum += _wspd;
      _wspd_count++;

      _wdir_array[_wdir_next()] = _read_wind_dir();
    }
    
    void calc() {
      wdir = _wdir_average();
      gust = _gust; //highest 5 second run over 5 minutes;
      wspd = _wspd_sum / _wspd_count; //Average over 5 minutes
      #ifdef DEBUG
        Serial.println(wdir);
        Serial.println(gust);
        Serial.println(wspd);
      #endif

      //resetting recorders
      _gust = 0;
      _wspd_sum = 0;
      _wspd_count = 0;
    }
};


#endif

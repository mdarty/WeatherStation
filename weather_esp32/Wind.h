unsigned long wspd_count = 0;
volatile unsigned long wspd_last_millis = millis();
volatile unsigned long wspd_last_micros = micros();

void wspd_inc() {
  if((long)(micros() - wspd_last_micros) >= DEBOUNCING_TIME) {
    wspd_count++;
    wspd_last_micros = micros();
  }
}

class WindMath {
  private:
    unsigned int _gust;
    unsigned int _wspd_count;
    unsigned int _wspd_sum;
    unsigned int _wspd;

    byte read_wind_dir() {
        //https://cdn.sparkfun.com/assets/d/1/e/0/6/DS-15901-Weather_Meter.pdf
        switch (analogRead(WDIR_PIN)) {
          case 0 ... 1208:
            return 270;
          case 1209 ... 1727:
            return 315;
          case 1728 ... 2087:
            return 293; // 292.5;
          case 2088 ... 2426:
            return 000;
          case 2427 ... 2777:
            return 338; //337.5;
          case 2778 ... 2971:
            return 225;
          case 2972 ... 3210:
            return 248; //247.5;
          case 3211 ... 3457:
            return 045;
          case 3458 ... 3622:
            return 023; //22.5;
          case 3623 ... 3761:
            return 180;
          case 3762 ... 3835:
            return 203; //202.5;
          case 3836 ... 3913:
            return 135;
          case 3914 ... 3969:
            return 158; //157.5;
          case 3970 ... 3994:
            return 90;
          case 3995 ... 4009:
            return 68; //67.5;
          case 4010 ... 4095:
            return 113; //112.5;
        }
    }
  
  public:
    unsigned int wdir;
    unsigned int gust;
    unsigned int wspd;

    String getAPRS() {
      return format(wdir) + "/" + format(wspd) + "g" + format(gust);
    }

    void collect_sample() {
      // Speed 2.4km/h (1.5mph) is 1 count per second
      // 2.4km/hr
      // 1.49129 mph * 1,000,000 macroseconds
      // 1.49129 mph * 1000 milliseconds
      // micros unsigned long can't handle 5 seconds plus I only need 3 sigfigs.
      // https://cdn.sparkfun.com/assets/d/1/e/0/6/DS-15901-Weather_Meter.pdf
      _wspd = 0.5 + ((1491290 * wspd_count) / (millis() - wspd_last_millis)); //0.5 rounds properly.

      #ifdef DEBUG
        Serial.print("wspd: ");
        Serial.println(_wspd);
        Serial.print("wspd_count: ");
        Serial.println(wspd_count);
        Serial.print("millis");
        Serial.println(millis());
        Serial.print("last_millis");
        Serial.print(wspd_last_millis);
      #endif

        wspd_last_millis = 0;
      if (_gust < _wspd){
        _gust = _wspd;
      }
      _wspd_sum += _wspd;
      _wspd_count++;
    }
    
    void calc() {
      wdir = read_wind_dir();
      gust = _gust; //highest 5 second run over 5 minutes;
      _gust = 0; //using private variable to keep
      wspd = _wspd_sum / _wspd_count; //Average over 5 minutes
    }
};

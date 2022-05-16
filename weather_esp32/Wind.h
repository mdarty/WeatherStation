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
        //10k resister
        switch (analogRead(WDIR_PIN)) {
          case 3002 ... 3234:
            return 0;
          case 1407 ... 1739:
            return 23;
          case 1740 ... 2159:
            return 45;
          case 300 ... 353:
            return 68;
          case 354 ... 439:
            return 90;
          case 0 ... 299:
            return 113;
          case 626 ... 862:
            return 135;
          case 440 ... 625:
            return 158;
          case 1066 ... 1406:
            return 180;
          case 863 ... 1065:
            return 203;
          case 2462 ... 2680:
            return 225;
          case 2160 ... 2461:
            return 248;
          case 3696 ... 4096:
            return 270;
          case 3235 ... 3450:
            return 293;
          case 3451 ... 3695:
            return 315;
          case 2681 ... 3001:
            return 338;
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

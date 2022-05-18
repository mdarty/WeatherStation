unsigned int rain_count = 0;
unsigned int rain_count_mid = 0;
unsigned long r_mid_change = 0;
volatile unsigned long rain_last_micros;
const unsigned int _24_len = 86400000/report_time; //86,400,000 milliseconds in 24 hours, sampled every 5 minutes
const unsigned int _hour_len = 3600000/report_time; //3,600,000 milliseconds in an hour, sampled every 5 minutes
//#define MID_WAIT 3600000 // don't even look for an hour

void rain_inc() {
  if((long)(micros() - rain_last_micros) >= DEBOUNCING_TIME) {
    rain_count++;
    rain_count_mid++;
    rain_last_micros = micros();
  }
}

//Keep everything as count until publishing.
class RainMath {
  // bucket_size = 0.2794mm 0.011inches
  // Rain should be in hundredths of an inch
  private:
    unsigned int _24_array[_24_len] = {0};
    unsigned int _24_index = 0;
    const float _bucket = 1.1; //hundreths of an inch
    volatile unsigned long _last_24;
    unsigned int _hour_array[_hour_len] = {0};
    volatile unsigned long _last_hour;
    byte _hour_index = 0;

    
    bool IsDST(byte month, byte day, byte dow) {
      //Return DaylightSavingsTime is true or false
      if (month < 3 || month > 11) {
        return false;
      }
      if (month > 3 && month < 11) {
        return true;
      }
      int previousSunday = day - dow;
      if ((month == 3) && (previousSunday >= 8)) {
        return true;
      }
      return false;
    }


    void _r_mid (int min, int hr, int month, int day, int dow) {
      //      char tmid[7] = "050000z"
      //      char fmid[7] = "060000z"
      //Checks daylight savings time and when local time is midnight it resets counter to zero and restarts timer.
      if (min == 0) {
        if (((hr == 5) && (IsDST(month, day, dow))) || ((hr == 6) && (!IsDST(month, day, dow)))) {
          rain_count_mid = 0;
          r_mid_changed = millis();
        }
      }
      r_mid = (_bucket * rain_count_mid) + 0.5;
    }

    int _24_next() {
      //circular index for 24 hours
      _24_index++;
      if (_24_index > _24_len) {
        _24_index = 0;
      }
      return _24_index;
    }

    void _r_24_sum(){
      //Last rain fall for 24 hours
      unsigned int rain_24 = 0;
      _24_array[_24_next()] = rain_count;
      
      // byte isn't large enough
      for (unsigned int i = 0; i < _24_len; i++) {
        rain_24 += _24_array[i];
      }
      r_24_sum = rain_24 * _bucket + 0.5; // 0.5 + rounds properly when turning to int
    }

    int _hour_next() {
      //circular index for 24 hours
      _hour_index++;
      if (_hour_index > _hour_len) {
        _hour_index = 0;
      }
      return _hour_index;
    }

    void _r_hour_sum(){
      //Last rain fall for 24 hours
      unsigned int rain_hour = 0;
      _hour_array[_hour_next()] = rain_count;
      
      // byte isn't large enough
      for (unsigned int i = 0; i < _24_len; i++) {
        rain_hour += _hour_array[i];
      }
      r_hour_sum = rain_hour * _bucket + 0.5; // 0.5 + rounds properly when turning to int
    }

    void _soil_moisture(){
      float V = map(analogRead(SOIL_PIN), 0, 4096, 0, 3.3);
      if (V > 0.8) {
        soil_moisture = 0;
      } else if (V < 0.46) {
        soil_moisture = 100;
      } else {
        soil_moisture = (0.8-V) / (0.8-0.46) * 100.0;
      }
    }
        
  public:
    volatile unsigned long r_mid_changed;
    unsigned int r_hour_sum;
    unsigned int r_24_sum;
    unsigned int r_mid;
    unsigned int soil_moisture;

    String getAPRS() {
      //Generate Rain portion of APRS string
      return "r" + format(r_hour_sum) + "p" + format(r_24_sum) + "P" + format(r_mid);
    }

    void calc(int min, int hr, int month, int day, int dow) {
      //take sample
      _r_hour_sum();
      
      post.println("Rain: Midnight");
      
      _r_mid (min, hr, month, day, dow);
      
      post.println("Rain: 24");
      
      _r_24_sum();
      rain_count = 0;

      _soil_moisture();
    }
};

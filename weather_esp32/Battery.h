#ifndef BATTERY_H_
#define BATTERY_H_

unsigned int bat_mV = BULK_CH_SP;
unsigned int solar_mV;
const unsigned int FLOAT_LOW = FLOAT_CH_SP - 400;
unsigned int dutyCycle = 0;
const unsigned int freq = 500;
const unsigned int M_Channel = 0;
const unsigned int resolution = 8;
volatile unsigned long hour_timer = millis();
const unsigned int hour_const = 3600000;
volatile unsigned long ten_min_timer = millis();
const unsigned int ten_min_const = 600000;
bool ten_started = false;

byte charge_mode = 0;
byte charge_mode_out = 0;
/*  0: OFF
 *  1: Bulk charge
 *  2: Absorption charge
 *  3: Float charge
 */

void bat_setup() {
  ledcSetup(M_Channel, freq, resolution);
  ledcAttachPin(PWM_C, M_Channel);
}

float readChannel(ADS1115_MUX channel) {
  float voltage = 0.0;
  adc.setCompareChannels(channel);
  adc.startSingleMeasurement();
  #ifdef DEBUG
  Serial.print("Bat Measure: ");
  while (adc.isBusy()) {Serial.print(".");}
  Serial.println(" ");
  #else
  while (adc.isBusy()) {}
  #endif
  voltage = adc.getResult_mV(); // alternative: getResult_mV for Millivolt
  return voltage;
}

void bulk_charge() {
  // from low to 14.4V
  if (bat_mV >= BULK_CH_SP) {
    dutyCycle = 0;
    charge_mode = 2;
    hour_timer = millis();
  }
  dutyCycle = 100;
}

void Ab_charge() {
  // keep at 14.4V for 1 hour
  if (millis() - hour_timer > hour_const) {
    charge_mode = 3;
    dutyCycle = 0;
  } else if (bat_mV >= BULK_CH_SP) {
    dutyCycle = 0;
  } else {
    int diff = BULK_CH_SP - bat_mV;
    if (diff >= 100) {
      dutyCycle = 100;
    } else {
      dutyCycle = diff;
    }
  }
}

void float_charge() {
  // if at 13.2 charge for 10 minutes 
  // keep below 13.5
  if (FLOAT_CH_SP < bat_mV) {
    dutyCycle = 0;
  } else if ((FLOAT_LOW >= bat_mV) & (!ten_started)) {
    //Wait 10 minutes
    ten_started = true;
    ten_min_timer = millis();
  } else if (ten_started & (millis() - ten_min_timer > ten_min_const) & (FLOAT_CH_SP < bat_mV)) {
    dutyCycle = 100;
  } else if (FLOAT_CH_SP >= bat_mV) {
    ten_started = false;
    dutyCycle = 0;
  }
}

void bat_charge() {
  solar_mV = readChannel(solar_pin) * 110/100;
  bat_mV = readChannel(bat_pin) * 110/100;
  batteryLevel = map(bat_mV, FLOAT_CH_SP * 0.85, FLOAT_CH_SP, 0, 100);

  if (bat_mV < FLOAT_LOW - 1000) {
    charge_mode = 1;
  }

  switch(charge_mode) {
    case 0 :
      dutyCycle = 0;
      break;
    case 1 :
      bulk_charge();
      break;
    case 2 :
      Ab_charge();
      break;
    case 3 :
      float_charge();
      break;
   }

  // This turns charging off for reasons without loosing charge mode. 
  // If a thick cloud passes over
  if ((bat_mV > solar_mV)) {
    dutyCycle = 0;
    charge_mode_out = 0;
  } else {
    charge_mode_out = charge_mode;
  }
  
  byte dutyCycleByte = map(dutyCycle, 0, 100, 0, 255);
  ledcWrite(M_Channel, dutyCycleByte);
}

#endif

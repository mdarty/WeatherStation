void I2C_read() {
  bme.setMode(MODE_FORCED);
  while (bme.isMeasuring() == false) ; //Wait for sensor to start measurment
  while (bme.isMeasuring() == true) ; //Hang out while sensor completes the reading
  temp = bme.readTempF();
  pressure = (bme.readFloatPressure() / 10) + 0.5;
  Serial.println(bme.readFloatPressure());
  Serial.println(pressure);
  humidity = bme.readFloatHumidity() + 0.5; //0.5 rounds properly
  bme.setMode(MODE_SLEEP);
}


w_data load_data() {
  w_data ws;
  strcpy(ws.Time, rtc.getTime("%FT%T").c_str());
  ws.temp = temp;
  ws.humidity = humidity;
  ws.pressure = pressure;
  ws.pressure = Wind.wdir;
  ws.gust = Wind.gust;
  ws.wspd = Wind.wspd;
  ws.r_hour_sum = Rain.r_hour_sum;
  ws.r_24_sum = Rain.r_24_sum;
  ws.r_mid = Rain.r_mid;
  ws.soil_moisture = Rain.soil_moisture;
  ws.batteryLevel = batteryLevel;
  ws.dutyCycle = dutyCycle;
  ws.solar_mV = solar_mV;
  ws.bat_mV = bat_mV;
  switch(charge_mode_out) {
    case 0 :
      ws.charge_mode = "OFF";
      break;
    case 1 :
      ws.charge_mode = "Bulk charge";
      break;
    case 2 :
      ws.charge_mode = "Absorption charge";
      break;
    case 3 :
      ws.charge_mode = "Float charge";
      break;
   }
  return ws;
}

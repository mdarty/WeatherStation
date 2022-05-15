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

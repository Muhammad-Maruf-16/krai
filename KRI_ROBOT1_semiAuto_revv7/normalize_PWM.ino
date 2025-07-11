void normalizePWM() { //batasi pwm dalam range yang aman.....
  int maxVal = max(max(abs(deRPM), abs(beRPM)), max(abs(kaRPM), abs(kiRPM)));

  if (maxVal > MAX_PWM) {
    float scaleFactor = (float)MAX_PWM / maxVal;
    deRPM *= scaleFactor;
    beRPM *= scaleFactor;
    kaRPM *= scaleFactor;
    kiRPM *= scaleFactor;
  }

  deRPM = constrain(deRPM, -MAX_PWM, MAX_PWM);
  beRPM = constrain(beRPM, -MAX_PWM, MAX_PWM);
  kaRPM = constrain(kaRPM, -MAX_PWM, MAX_PWM);
  kiRPM = constrain(kiRPM, -MAX_PWM, MAX_PWM);
}
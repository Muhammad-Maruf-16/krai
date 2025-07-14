//program untuk mengatur arah gerakan motor
void DE_DIRECTION(int dir) {
  switch (dir) {
    case CW:
      analogWrite(DE_RPWM_PIN, round(PWM_DE));
      digitalWrite(DE_LPWM_PIN, LOW);
      break;
    case CCW:
      digitalWrite(DE_RPWM_PIN, LOW);
      analogWrite(DE_LPWM_PIN, round(PWM_DE));
      break;
    case Stop:
      digitalWrite(DE_RPWM_PIN, LOW);
      digitalWrite(DE_LPWM_PIN, LOW);
      break;
  }
}
void BE_DIRECTION(int dir) {
  switch (dir) {
    case CW:
      analogWrite(BE_RPWM_PIN, round(PWM_BE));
      digitalWrite(BE_LPWM_PIN, LOW);
      break;
    case CCW:
      digitalWrite(BE_RPWM_PIN, LOW);
      analogWrite(BE_LPWM_PIN, round(PWM_BE));
      break;
    case Stop:
      digitalWrite(BE_RPWM_PIN, LOW);
      digitalWrite(BE_LPWM_PIN, LOW);
      break;
  }
}
void KA_DIRECTION(int dir) {
  switch (dir) {
    case CW:
      analogWrite(KA_RPWM_PIN, round(PWM_KA));
      digitalWrite(KA_LPWM_PIN, LOW);
      break;
    case CCW:
      digitalWrite(KA_RPWM_PIN, LOW);
      analogWrite(KA_LPWM_PIN, round(PWM_KA));
      break;
    case Stop:
      digitalWrite(KA_RPWM_PIN, LOW);
      digitalWrite(KA_LPWM_PIN, LOW);
      break;
  }
}
void KI_DIRECTION(int dir) {
  switch (dir) {
    case CW:
      analogWrite(KI_RPWM_PIN, round(PWM_KI));
      digitalWrite(KI_LPWM_PIN, LOW);
      break;
    case CCW:
      digitalWrite(KI_RPWM_PIN, LOW);
      analogWrite(KI_LPWM_PIN, round(PWM_KI));
      break;
    case Stop:
      digitalWrite(KI_RPWM_PIN, LOW);
      digitalWrite(KI_LPWM_PIN, LOW);
      break;
  }
}




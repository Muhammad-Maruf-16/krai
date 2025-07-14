//program untuk percobaan membaca rpm

void readRPM(int Speed) {
  float RPM, RPM1;
  int PULSE;

  analogWrite(DE_RPWM_PIN, Speed);
  digitalWrite(DE_LPWM_PIN, LOW);

  if ((millis() - Previousmillis) >= 1000) {
    Previousmillis = millis();
    noInterrupts();
    PULSE = ENC_DE_COUNT;
    ENC_DE_COUNT = 0;
    interrupts();
    RPM = float((PULSE * 60.0) / 134);
    RPM1 = float((PULSE / 134.0) * 60);
    Serial.print(RPM);
    Serial.print("  ");
    Serial.println(RPM1);
    // ENC_DE_COUNT = 0;
  }
}
/* dengan pembacaan setiap 1s
depan 271 460
belakang 233 560
kanan 222
kiri 259



pembacaan setiap 0.1s
gak stabil
*/
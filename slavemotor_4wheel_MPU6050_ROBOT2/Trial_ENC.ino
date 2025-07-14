//program untuk mencoba encoder
void trialEnc() {
  // while (1) {
  Serial.print("BE_ENC: ");
  Serial.print(ENC_BE_COUNT);
  Serial.print("\t");

  Serial.print("KA_ENC: ");
  Serial.print(ENC_KA_COUNT);
  Serial.print("\t");

  Serial.print("KI_ENC: ");
  Serial.println(ENC_KI_COUNT);
  delay(100);
  // }
}
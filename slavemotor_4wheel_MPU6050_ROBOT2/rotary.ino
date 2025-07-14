//program untuk rotary encoder
void ENC_DE_FUNCT() {
  digitalRead(ENC_DE_CHB) ? ENC_DE_COUNT-- : ENC_DE_COUNT++;
}

void ENC_BE_FUNCT() {
  digitalRead(ENC_BE_CHB) ? ENC_BE_COUNT-- : ENC_BE_COUNT++;
}

void ENC_KA_FUNCT() {
  digitalRead(ENC_KA_CHB) ? ENC_KA_COUNT-- : ENC_KA_COUNT++;
}

void ENC_KI_FUNCT() {
  digitalRead(ENC_KI_CHB) ? ENC_KI_COUNT-- : ENC_KI_COUNT++;
}

// void lSAtas_FUNCT(){
//   limitAtasAktif = true;
// }

// void lSBawah_FUNCT(){
//   limitBawahAktif = true;
// }

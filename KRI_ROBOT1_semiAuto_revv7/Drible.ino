unsigned long solenoid1_time = 0;  //waktu aktif solenoid1 (sol1)
unsigned long solenoid2_time = 0;  //waktu aktif solenoid2 (sol2)
unsigned long delayTime_time = 0;  //delay time solenoid 1 nyala sekian detik setelah solenoid 2
bool drible_on = false;
bool flagSol2 = false;

void driblee() {

  if (square_) drible_on = true;               //drible
  else if (circle && !alreadyPressedCIRCLE) {  //buka tutup gripper
    alreadyPressedCIRCLE = true;
    flagSol2 = !flagSol2;
    sol2 = flagSol2;
  } else {
    if (!circle) { alreadyPressedCIRCLE = false; }
    if (!drible_on && !flagSol2) sol2 = 0;
  }

  if (drible_on) {
    if (millis() - solenoid1_time >= Solenoid1) sol1 = 0;
    else {
      if (millis() - delayTime_time >= delayTime) sol1 = 1;
    }
    if (millis() - solenoid2_time >= Solenoid2) {
      sol2 = 0;
      drible_on = false;
    } else sol2 = 1;
  } else {
    solenoid1_time = millis();
    solenoid2_time = millis();
    delayTime_time = millis();
    sol1 = 0;
    if (!circle && !flagSol2) {
      sol2 = 0;
    }
  }
}
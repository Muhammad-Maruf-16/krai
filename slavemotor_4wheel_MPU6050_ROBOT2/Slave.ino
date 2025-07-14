//data dari master harus dikirim terus menerus agar motor bergerak, jika hanya dikirim sekali maka motor tidak bergerak
//itu kelemahan program sekarang, perlu dikembangkan

int Loss = 0;
int filtered_deRPM = 0, filtered_beRPM = 0, filtered_kaRPM = 0, filtered_kiRPM = 0;

// Deteksi apakah sedang dalam mode perlambatan (target = 0 atau perubahan arah)
bool isBrakingOrDirectionChange(int current, int target) {
  return (target == 0) || (current > 0 && target < 0) || (current < 0 && target > 0);
}

int rampRPM(int current, int target) {
  int step;
  int delta = abs(target - current);

  if (abs(current) < 10 && current != 0) {
    current = 0;
    return current;
  }

  if (target == 0) {
    if (abs(current) <= 5) return 0;
    step = max(3, (int)(abs(current) * 0.10));  // 10% per step
    if (current > 0) {
      current = max(0, current - step);
    } else if (current < 0) {
      current = min(0, current + step);
    }
    if (abs(current) < 5) current = 0;
    return current;
  }

  // perubahan arah
  if ((current > 0 && target < 0) || (current < 0 && target > 0)) {
    int total_distance = abs(current) + abs(target);
    step = max(min(abs(current) / 4, abs(target) / 3), 5);
    step = min(step, 25); //max step 25
    if (total_distance < 30) step = max(2, step / 2);  // Haluskan untuk jarak pendek
    
    if (current > target) {
      current -= step;
      if (current < target) current = target;
    } else {
      current += step;
      if (current > target) current = target;
    }
    return current;
  }

  // Akselerasi normal
  if (delta > 150) step = 15;
  else if (delta > 100) step = 10;
  else if (delta > 50) step = 5;
  else if (delta > 20) step = 2;
  else step = 1;
  if (current < target) {
    current += step;
    if (current > target) current = target;
  } else if (current > target) {
    current -= step;
    if (current < target) current = target;
  }
  return current;
}


//loop slave
void vSlave() {
  bool stateUp = false;
  String
    Data,
    CMD, sVx, sVy, sVv;
  int
    deRPM = 0,
    beRPM = 0,
    kaRPM = 0,
    kiRPM = 0,
    Vx = 0,
    Vy = 0,
    Vv;

  int
    nA,
    nB,
    nC,
    nD,
    nE,
    nF;


  while (true) {
    if (slaveSerial.available()) {
      Data = slaveSerial.readStringUntil('\n');

      int nA = Data.indexOf("A");
      int nB = Data.indexOf("B");
      int nC = Data.indexOf("C");
      int nD = Data.indexOf("D");
      int nE = Data.indexOf("E");
      int nF = Data.indexOf("F");
      int nG = Data.indexOf("G");
      int nH = Data.indexOf("H");
      int nI = Data.indexOf("I");

      String sDe = Data.substring(nA + 1, nB);
      String sBe = Data.substring(nB + 1, nC);
      String sKa = Data.substring(nC + 1, nD);
      String sKi = Data.substring(nD + 1, nE);
      String sUp = Data.substring(nE + 1, nF);
      String sDown = Data.substring(nF + 1, nG);
      String sLoss = Data.substring(nG + 1, nH);
      String sBost = Data.substring(nH + 1, nI);

      deRPM = sDe.toInt();
      beRPM = sBe.toInt();
      kaRPM = sKa.toInt();
      kiRPM = sKi.toInt();

      // Batasan PWM
      deRPM = constrain(deRPM, -750, 750);
      beRPM = constrain(beRPM, -750, 750);
      kaRPM = constrain(kaRPM, -750, 750);
      kiRPM = constrain(-kiRPM, -750, 750);

      filtered_deRPM = rampRPM(filtered_deRPM, deRPM);
      filtered_beRPM = rampRPM(filtered_beRPM, beRPM);
      filtered_kaRPM = rampRPM(filtered_kaRPM, kaRPM);
      filtered_kiRPM = rampRPM(filtered_kiRPM, kiRPM);

      Loss = sLoss.toInt();
      int bost = sBost.toInt();
      float bostf = (float)bost / 100.0;
      if (Loss) {
        filtered_deRPM = deRPM;
        filtered_beRPM = beRPM;
        filtered_kaRPM = kaRPM;
        filtered_kiRPM = kiRPM;
      }


      // Kirim ke motor
      // RPMdepan(deRPM, 0.1, 0.0, 0.0);
      // RPMbelakang(beRPM, 0.107, 0.0, 0.15);
      // RPMkanan(kaRPM, 0.12, 0.0, 0.06);
      // RPMkiri(kiRPM, 0.115, 0.0, 0.0);

      RPMdepan(filtered_deRPM, bostf, 0.0001, 0.1);
      RPMbelakang(filtered_beRPM, bostf, 0.0001, 0.1);
      RPMkanan(filtered_kaRPM, bostf, 0.0001, 0.1);
      RPMkiri(filtered_kiRPM, bostf, 0.0001, 0.1);

      // RPMdepan(deRPM, 0.05, 0.0002, 0.5);
      // RPMbelakang(beRPM, 0.05, 0.0002, 0.5);
      // RPMkanan(kaRPM, 0.05, 0.0002, 0.5);
      // RPMkiri(kiRPM, 0.05, 0.0002, 0.45);
      int upState = sUp.toInt();
      if (upState) stateUp = true;
      int downState = sDown.toInt();
      if (downState) stateUp = false;
      // Serial.println(upState);
      if (digitalRead(lSAtas_PIN)) {
        // limitBawahAktif = false;
        digitalWrite(motorUp, stateUp);
      } else {
        digitalWrite(motorUp, 0);
        stateUp = false;
      }
      // limitAtasAktif = false;
      digitalWrite(motorDown, downState);
      // Serial.print(limitAtasAktif);
      // Serial.println(limitBawahAktif);
    }
  }
}
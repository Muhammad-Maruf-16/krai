//calculate dpad 
//update berikutnya ubah pakai invers kinematics 4 wheel

void calculateDpadMovement() {
  deRPM = beRPM = kaRPM = kiRPM = 0;
  deRPM1 = beRPM1 = kaRPM1 = kiRPM1 = 0;

  if (up && !down || stickLY < 20) {
    if (left && !right) {
      deRPM = 0;
      beRPM = 0;
      kaRPM = -DPAD_SPEED + kaOffset;
      kiRPM = DPAD_SPEED - kiOffset;
    } else if (right && !left) {
      deRPM = DPAD_SPEED - deOffset;
      beRPM = -DPAD_SPEED + beOffset;
      kaRPM = 0;
      kiRPM = 0;
    } else if (stickRX < 20) {
      deRPM = DPAD_SPEED + M4;
      beRPM = -DPAD_SPEED - M2;
      kaRPM = -DPAD_SPEED - M3;
      kiRPM = DPAD_SPEED + M1;
    } else if (stickRX > 240) {
      deRPM = DPAD_SPEED + M3;
      beRPM = -DPAD_SPEED - M1;
      kaRPM = -DPAD_SPEED - M4;
      kiRPM = DPAD_SPEED + M2;
    } else {
      deRPM = DPAD_SPEED - deOffset;
      beRPM = -DPAD_SPEED + beOffset;
      kaRPM = -DPAD_SPEED + kaOffset;
      kiRPM = DPAD_SPEED - kiOffset;
    }

    // deRPM += gyroCorrection_deRPM;
    // beRPM += gyroCorrection_beRPM;
    // kaRPM += gyroCorrection_kaRPM;
    // kiRPM += gyroCorrection_kiRPM;
  } else if (down && !up || stickLY > 240) {
    if (right && !left) {
      deRPM = 0;
      beRPM = 0;
      kaRPM = DPAD_SPEED - kaOffset;
      kiRPM = -DPAD_SPEED + kiOffset;
    } else if (left && !right) {
      deRPM = -DPAD_SPEED + deOffset;
      beRPM = DPAD_SPEED - beOffset;
      kaRPM = 0;
      kiRPM = 0;
    } else if (stickRX > 240) {
      deRPM = -DPAD_SPEED - M1;
      beRPM = DPAD_SPEED + M3;
      kaRPM = DPAD_SPEED + M2;
      kiRPM = -DPAD_SPEED - M4;
    } else if (stickRX < 20) {
      deRPM = -DPAD_SPEED - M2;
      beRPM = DPAD_SPEED + M4;
      kaRPM = DPAD_SPEED + M1;
      kiRPM = -DPAD_SPEED - M3;
    } else {
      deRPM = -DPAD_SPEED + deOffset;
      beRPM = DPAD_SPEED - beOffset;
      kaRPM = DPAD_SPEED - kaOffset;
      kiRPM = -DPAD_SPEED + kiOffset;
    }

    // deRPM += gyroCorrection_deRPM;
    // beRPM += gyroCorrection_beRPM;
    // kaRPM += gyroCorrection_kaRPM;
    // kiRPM += gyroCorrection_kiRPM;
  } else if (left && !right || stickLX < 20) {
    deRPM = -DPAD_SPEED + deOffset;
    beRPM = DPAD_SPEED - beOffset;
    kaRPM = -DPAD_SPEED + kaOffset;
    kiRPM = DPAD_SPEED - kiOffset;
    // deRPM += gyroCorrection_deRPM;
    // beRPM += gyroCorrection_beRPM;
    // kaRPM += gyroCorrection_kiRPM;
    // kiRPM += gyroCorrection_kaRPM;
  } else if (right && !left || stickLX > 240) {
    deRPM = DPAD_SPEED - deOffset;
    beRPM = -DPAD_SPEED + beOffset;
    kaRPM = DPAD_SPEED - kaOffset;
    kiRPM = -DPAD_SPEED + kiOffset;
    // deRPM += gyroCorrection_deRPM;
    // beRPM += gyroCorrection_beRPM;
    // kaRPM += gyroCorrection_kiRPM;
    // kiRPM += gyroCorrection_kaRPM;
  } else if (stickRY > 240) {
    deRPM = -DPAD_SPEED + deOffset;
    beRPM = -DPAD_SPEED + beOffset;
    kaRPM = -DPAD_SPEED + kaOffset;
    kiRPM = -DPAD_SPEED + kiOffset;
  } else if (stickRY < 20) {
    deRPM = DPAD_SPEED - deOffset;
    beRPM = DPAD_SPEED - beOffset;
    kaRPM = DPAD_SPEED - kaOffset;
    kiRPM = DPAD_SPEED - kiOffset;
  } else if (stickRX > 240) {
    deRPM = -slowRote + deOffset;
    beRPM = -slowRote + beOffset;
    kaRPM = -slowRote + kaOffset;
    kiRPM = -slowRote + kiOffset;
  } else if (stickRX < 20) {
    deRPM = slowRote - deOffset;
    beRPM = slowRote - beOffset;
    kaRPM = slowRote - kaOffset;
    kiRPM = slowRote - kiOffset;
  }

  //soft brake enable or disable
  if (L2 && !L1) {
    loss = 1;
  } else loss = 0;


  /*auto mode, proses pengembangan*/
  // deRPM += M4a;
  // beRPM += M1a;
  // kaRPM += M3a;
  // kiRPM += M2a;


  // if (xSemaphoreTake(motorRPM_mutex, portMAX_DELAY)) {
  // Terapkan koreksi langsung tanpa mutex
  //   xSemaphoreGive(motorRPM_mutex);
  // }

  
  normalizePWM(); //batasi pwm

  if (R1) { //bost pwm & Kp PID
    deRPM *= 2;
    beRPM *= 2;
    kaRPM *= 2;
    kiRPM *= 2;
    rawBost = 30;
  } else {
    rawBost = 5;
  }
}

// kalibrasi motor manual, jarang dan hampir tidak digunakan,..
void settingRPM() {
  if (L1) tesMovement();
  else {
    if (START && !alreadyPressedSTART) {
      alreadyPressedSTART = true;
      deOffset = beOffset = kaOffset = kiOffset = 0;
    } else if (!START) {
      alreadyPressedSTART = false;
    }
    
    if (millis() - lTime2 >= 200) {
      if (triangle) {
        if (up) deOffset += CALIBRATION_STEP;
        if (down) deOffset -= CALIBRATION_STEP;
      }
      // BE RPM adjustment (Cross)
      if (cross) {
        if (up) beOffset += CALIBRATION_STEP;
        if (down) beOffset -= CALIBRATION_STEP;
      }
      // KA RPM adjustment (Circle)
      if (circle) {
        if (up) kaOffset += CALIBRATION_STEP;
        if (down) kaOffset -= CALIBRATION_STEP;
      }
      // KI RPM adjustment (Square)
      if (square_) {
        if (up) kiOffset += CALIBRATION_STEP;
        if (down) kiOffset -= CALIBRATION_STEP;
      }
      lTime2 = millis();
    }
    // Constrain offsets to reasonable values
    deOffset = constrain(deOffset, -MAX_PWM, MAX_PWM);
    beOffset = constrain(beOffset, -MAX_PWM, MAX_PWM);
    kaOffset = constrain(kaOffset, -MAX_PWM, MAX_PWM);
    kiOffset = constrain(kiOffset, -MAX_PWM, MAX_PWM);



    //  tesMovement();
    // else {
    //   deRPM = 0;
    //   // beRPM = beOffset;
    //   beRPM = 0;
    //   // kaRPM = kaOffset;
    //   kaRPM = 0;
    //   // kiRPM = kiOffset;
    //   kiRPM = 0;
    // }

    /*print lcd*/
    if (millis() - lTime1 > 500) {
      lTime1 = millis();

      lcd.setCursor(0, 0);
      lcd.print("Set motor    ");

      lcd.setCursor(0, 1);
      lcd.print("MDKn:");
      lcd.print(deOffset);
      lcd.print("   ");

      lcd.setCursor(10, 1);
      lcd.print("MDKr:");
      lcd.print(kiOffset);
      lcd.print("  ");

      lcd.setCursor(0, 2);
      lcd.print("MBKn:");
      lcd.print(kaOffset);
      lcd.print("   ");

      lcd.setCursor(10, 2);
      lcd.print("MBKr:");
      lcd.print(beOffset);
      lcd.print("  ");
    }
    deRPM = 0;
    beRPM = 0;
    kaRPM = 0;
    kiRPM = 0;
  }
  sendToSlave();
}
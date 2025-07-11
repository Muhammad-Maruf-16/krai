uint8_t setting = 0;
unsigned long lTime2 = 0;
void handleSettingMode() {
  // Check if R3 is pressed to save and exit


  if (R3) {
    if (L1) {
      //do nothing
    } else {
      prefs.begin("robot_2", false);  // false = read-write mode
      prefs.putInt("deOffset", deOffset);
      prefs.putInt("beOffset", beOffset);
      prefs.putInt("kaOffset", kaOffset);
      prefs.putInt("kiOffset", kiOffset);
      prefs.putInt("M1", M1);
      prefs.putInt("M2", M2);
      prefs.putInt("M3", M3);
      prefs.putInt("M4", M4);
      prefs.putInt("slowRote", slowRote);
      prefs.putInt("M1B", M1B);
      prefs.putInt("M2B", M2B);
      prefs.putInt("M3B", M3B);
      prefs.putInt("M4B", M4B);
      prefs.putInt("slowRoteB", slowRoteB);

      prefs.end();
    }
    lcd.clear();
    lcd.print("SPEED ");
    lcd.print(DPAD_SPEED);
    settingMode = false;
  } else if (!R3) {
    alreadyPressedR3 = false;
  }

  if (L3 && !alreadyPressedL3) {
    alreadyPressedL3 = true;
    if (L1) {
      if (setting > 0) setting--;
      lcd.clear();
    } else {
      if (setting < 3) setting++;
      lcd.clear();
    }
  } else if (!L3) {
    alreadyPressedL3 = false;
  }

  static bool printed = false;
  if (setting == 0) settingRPM();
  else if (setting == 1) setBelok();
  else if (setting == 2) setBelok2();
  else if (setting == 3) setSlowRote();
}

void settingRPM() {

  if (L1) tesMovement();
  else {
    if (START) {
      deOffset = beOffset = kaOffset = kiOffset = 0;
    } else if (millis() - lTime2 >= 200) {
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

    // /*print lcd*/
    if (millis() - lTime1 > 500) {
      lTime1 = millis();

      lcd.setCursor(0, 0);
      lcd.print("MDKn:");
      lcd.print(deOffset);
      lcd.print("   ");

      lcd.setCursor(8, 0);
      lcd.print("MDKr:");
      lcd.print(kiOffset);
      lcd.print("  ");

      lcd.setCursor(0, 1);
      lcd.print("MBKn:");
      lcd.print(kaOffset);
      lcd.print("   ");

      lcd.setCursor(8, 1);
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


uint8_t select_Index = 0;
void setBelok() {
  if (L1) calculateDpadMovement();
  else {
    if (START && !alreadyPressedSTART) {
      alreadyPressedSTART = true;
      M1 = M2 = M3 = M4 = 0;
    } else if (!START) {
      alreadyPressedSTART = false;
    }
    if (down && !alreadyPressedDOWN) {
      alreadyPressedDOWN = true;
      select_Index++;
      if (select_Index > 3) select_Index = 0;
    } else if (!down) {
      alreadyPressedDOWN = false;
    }
    if (up && !alreadyPressedUP) {
      alreadyPressedUP = true;
      if (select_Index > 0) select_Index--;
    } else if (!up) {
      alreadyPressedUP = false;
    }


    if (right && !alreadyPressedRIGHT) {
      alreadyPressedRIGHT = true;

      if (select_Index == 0) M1 += 10;
      else if (select_Index == 1) M2 += 10;
      else if (select_Index == 2) M3 += 10;
      else if (select_Index == 3) M4 += 10;
      // else slowRote += 10;

    } else if (!right) {
      alreadyPressedRIGHT = false;
    }
    if (left && !alreadyPressedLEFT) {
      alreadyPressedLEFT = true;

      if (select_Index == 0) M1 -= 10;
      else if (select_Index == 1) M2 -= 10;
      else if (select_Index == 2) M3 -= 10;
      else if (select_Index == 3) M4 -= 10;
      // else slowRote -= 10;

    } else if (!left) {
      alreadyPressedLEFT = false;
    }

    if (millis() - lTime3 > 200) {
      lTime3 = millis();
      blinkState = !blinkState;
      const uint8_t positions[4][2] = {
        { 6, 0 },   // set_drible == 0
        { 6, 1 },   // set_drible == 1
        { 14, 0 },  // set_drible == 2
        { 14, 1 }   // set_drible == 3
      };

      lcd.setCursor(positions[select_Index][0], positions[select_Index][1]);
      lcd.print(blinkState ? "-+" : "  ");
      lcd.blink();
    }

    if (millis() - lTime1 > 500) {
      lTime1 = millis();

      lcd.setCursor(0, 0);
      lcd.print("Set Belok ");

      lcd.setCursor(0, 0);
      lcd.print("M1:");
      lcd.print(M1);
      lcd.print("  ");

      lcd.setCursor(8, 0);
      lcd.print("M3:");
      lcd.print(M3);
      lcd.print("  ");

      lcd.setCursor(0, 1);
      lcd.print("M2:");
      lcd.print(M2);
      lcd.print("  ");

      lcd.setCursor(8, 1);
      lcd.print("M4:");
      lcd.print(M4);
      lcd.print("  ");
    }
    deRPM = 0;
    beRPM = 0;
    kaRPM = 0;
    kiRPM = 0;
  }
  sendToSlave();
}

void setBelok2() {
  if (L1) calculateDpadMovement();
  else {
    if (START && !alreadyPressedSTART) {
      alreadyPressedSTART = true;
      M1B = M2B = M3B = M4B = 0;
      // slowRoteB = 150;
    } else if (!START) {
      alreadyPressedSTART = false;
    }
    if (down && !alreadyPressedDOWN) {
      alreadyPressedDOWN = true;
      select_Index++;
      if (select_Index > 3) select_Index = 0;
    } else if (!down) {
      alreadyPressedDOWN = false;
    }
    if (up && !alreadyPressedUP) {
      alreadyPressedUP = true;
      if (select_Index > 0) select_Index--;
    } else if (!up) {
      alreadyPressedUP = false;
    }


    if (right && !alreadyPressedRIGHT) {
      alreadyPressedRIGHT = true;

      if (select_Index == 0) M1B += 10;
      else if (select_Index == 1) M2B += 10;
      else if (select_Index == 2) M3B += 10;
      else if (select_Index == 3) M4B += 10;
      // else slowRoteB += 10;

    } else if (!right) {
      alreadyPressedRIGHT = false;
    }
    if (left && !alreadyPressedLEFT) {
      alreadyPressedLEFT = true;

      if (select_Index == 0) M1B -= 10;
      else if (select_Index == 1) M2B -= 10;
      else if (select_Index == 2) M3B -= 10;
      else if (select_Index == 3) M4B -= 10;
      // else slowRoteB -= 10;

    } else if (!left) {
      alreadyPressedLEFT = false;
    }

    if (millis() - lTime3 > 200) {
      lTime3 = millis();
      blinkState = !blinkState;
      const uint8_t positions[4][2] = {
        { 6, 0 },   // set_drible == 0
        { 6, 1 },   // set_drible == 1
        { 14, 0 },  // set_drible == 2
        { 14, 1 }   // set_drible == 3
      };

      lcd.setCursor(positions[select_Index][0], positions[select_Index][1]);
      lcd.print(blinkState ? "-+" : "  ");
      lcd.blink();
    }

    if (millis() - lTime1 > 500) {
      lTime1 = millis();

      lcd.setCursor(0, 0);
      lcd.print("Set Belok ");

      lcd.setCursor(0, 0);
      lcd.print("M1B:");
      lcd.print(M1B);
      lcd.print("  ");

      lcd.setCursor(8, 0);
      lcd.print("M3B:");
      lcd.print(M3B);
      lcd.print("  ");

      lcd.setCursor(0, 1);
      lcd.print("M2B:");
      lcd.print(M2B);
      lcd.print("  ");

      lcd.setCursor(8, 1);
      lcd.print("M4B:");
      lcd.print(M4B);
      lcd.print("  ");
    }
    deRPM = 0;
    beRPM = 0;
    kaRPM = 0;
    kiRPM = 0;
  }
  sendToSlave();
}


void tesMovement() {
  if (up) {
    deRPM = DPAD_SPEED - deOffset;
    beRPM = -DPAD_SPEED + beOffset;
    kaRPM = -DPAD_SPEED + kaOffset;
    kiRPM = DPAD_SPEED - kiOffset;
  } else if (down) {
    deRPM = -DPAD_SPEED + deOffset;
    beRPM = DPAD_SPEED - beOffset;
    kaRPM = DPAD_SPEED - kaOffset;
    kiRPM = -DPAD_SPEED + kiOffset;
  } else if (left) {
    deRPM = DPAD_SPEED - deOffset;
    beRPM = -DPAD_SPEED + beOffset;
    kaRPM = DPAD_SPEED - kaOffset;
    kiRPM = -DPAD_SPEED + kiOffset;
  } else if (right) {
    deRPM = -DPAD_SPEED + deOffset;
    beRPM = DPAD_SPEED - beOffset;
    kaRPM = -DPAD_SPEED + kaOffset;
    kiRPM = DPAD_SPEED - kiOffset;
  } else {
    // deRPM = deOffset;
    deRPM = 0;
    // beRPM = beOffset;
    beRPM = 0;
    // kaRPM = kaOffset;
    kaRPM = 0;
    // kiRPM = kiOffset;
    kiRPM = 0;
  }
  normalizePWM();
  // sendToSlave();
}


// uint8_t set_drible = 0;

void setSlowRotelawasan() {
  if (L1) calculateDpadMovement();
  else {
    if (right & !alreadyPressedRIGHT) {
      slowRote += 10;
      alreadyPressedRIGHT = true;
    } else if (!right) {
      alreadyPressedRIGHT = false;
    }
    if (left & !alreadyPressedLEFT) {
      slowRote -= 10;
      alreadyPressedRIGHT = true;
    } else if (!left) {
      alreadyPressedRIGHT = false;
    }


    if (millis() - lTime3 > 200) {
      lTime3 = millis();
      blinkState = !blinkState;
    };

    lcd.setCursor(13, 1);
    lcd.print(blinkState ? "-+" : "  ");
    lcd.blink();
  }

  if (millis() - lTime1 > 500) {
    lTime1 = millis();

    lcd.setCursor(0, 0);
    lcd.print("Set Slow Rotasi");

    lcd.setCursor(0, 1);
    lcd.print("Rote:");
    lcd.print(slowRote);
    lcd.print("  ");
  }
  deRPM = 0;
  beRPM = 0;
  kaRPM = 0;
  kiRPM = 0;

  sendToSlave();
}

// sendToSlave();
void setSlowRote() {
  if (L1) {
    calculateDpadMovement();
  } else {
    if (START) {
      slowRote = 150;
      slowRoteB = 200;
    }

    if (down) select_Index = 1;
    else if (up) select_Index = 0;

    if (right && !alreadyPressedRIGHT) {
      alreadyPressedRIGHT = true;
      (select_Index == 0 ? slowRote : slowRoteB) += 10;
    } else if (!right) {
      alreadyPressedRIGHT = false;
    }

    if (left && !alreadyPressedLEFT) {
      alreadyPressedLEFT = true;
      (select_Index == 0 ? slowRote : slowRoteB) -= 10;
    } else if (!left) {
      alreadyPressedLEFT = false;
    }

    if (millis() - lTime3 > 200) {
      lTime3 = millis();
      blinkState = !blinkState;

      const uint8_t posX[] = { 6, 14 };  // posisi kursor untuk slowRote & slowRoteB
      lcd.setCursor(posX[select_Index], 1);
      lcd.print(blinkState ? "-+" : "  ");
    }

    // Update tampilan LCD
    if (millis() - lTime1 > 500) {
      lTime1 = millis();
      lcd.setCursor(0, 0);
      lcd.print("Set Slow Rotasi");

      lcd.setCursor(0, 1);
      lcd.print("RotA:");
      lcd.print(slowRote);
      lcd.print("  ");  // tambahan spasi untuk clear angka sebelumnya

      lcd.setCursor(8, 1);
      lcd.print("RotB:");
      lcd.print(slowRoteB);
      lcd.print("  ");
    }

    // Reset RPM
    deRPM = beRPM = kaRPM = kiRPM = 0;
  }

  sendToSlave();
}

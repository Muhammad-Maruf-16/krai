uint8_t select_Index = 0;

void resetBelok() {
  if (START && !alreadyPressedSTART) {
    alreadyPressedSTART = true;
    M1 = M2 = M3 = M4 = 0;
  } else if (!START) {
    alreadyPressedSTART = false;
  }
}

void selectIndexBelok() {
  if (down && !alreadyPressedDOWN) {
    alreadyPressedDOWN = true;
    select_Index++;
    if (select_Index > 4) select_Index = 0;
  } else if (!down) {
    alreadyPressedDOWN = false;
  }

  if (up && !alreadyPressedUP) {
    alreadyPressedUP = true;
    if (select_Index > 0) select_Index--;
  } else if (!up) {
    alreadyPressedUP = false;
  }
}

void setBelok() {
  if (L1) calculateDpadMovement();  //tes
  else {
    resetBelok();
    selectIndexBelok();


    if (right && !alreadyPressedRIGHT) {
      alreadyPressedRIGHT = true;

      if (select_Index == 0) M1 += 10;
      else if (select_Index == 1) M2 += 10;
      else if (select_Index == 2) M3 += 10;
      else if (select_Index == 3) M4 += 10;
      else slowRote += 10;

    } else if (!right) {
      alreadyPressedRIGHT = false;
    }

    if (left && !alreadyPressedLEFT) {
      alreadyPressedLEFT = true;

      if (select_Index == 0) M1 -= 10;
      else if (select_Index == 1) M2 -= 10;
      else if (select_Index == 2) M3 -= 10;
      else if (select_Index == 3) M4 -= 10;
      else slowRote -= 10;

    } else if (!left) {
      alreadyPressedLEFT = false;
    }



    //print ke lcd

    if (millis() - lTime3 > 200) {
      lTime3 = millis();
      blinkState = !blinkState;
      const uint8_t positions[5][2] = {
        { 8, 1 },   // set_drible == 0
        { 8, 2 },   // set_drible == 1
        { 18, 1 },  // set_drible == 2
        { 18, 2 },  // set_drible == 3
        { 13, 3 }   // set_drible == 1
      };

      lcd.setCursor(positions[select_Index][0], positions[select_Index][1]);
      lcd.print(blinkState ? "-+" : "  ");
      // lcd.blink();
    }

    if (millis() - lTime1 > 500) {
      lTime1 = millis();

      lcd.setCursor(0, 0);
      lcd.print("Set Belok ");

      lcd.setCursor(0, 1);
      lcd.print("M1:");
      lcd.print(M1);
      lcd.print("  ");

      lcd.setCursor(10, 1);
      lcd.print("M3:");
      lcd.print(M3);
      lcd.print("  ");

      lcd.setCursor(0, 2);
      lcd.print("M2:");
      lcd.print(M2);
      lcd.print("  ");

      lcd.setCursor(10, 2);
      lcd.print("M4:");
      lcd.print(M4);
      lcd.print("  ");

      lcd.setCursor(0, 3);
      lcd.print("Rote:");
      lcd.print(slowRote);
      lcd.print("  ");
    }
    deRPM = 0;
    beRPM = 0;
    kaRPM = 0;
    kiRPM = 0;
  }
  sendToSlave();
}
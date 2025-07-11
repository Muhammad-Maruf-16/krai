uint8_t set_drible = 0;

void setDrible() { //set drible
  if (L1) {

    driblee();
    sendToSlave();
  } else {
    if (START && !alreadyPressedSTART) {
      alreadyPressedSTART = true; 
      Solenoid1 = 300;
      Solenoid2 = 500;
      delayTime = 0;
    } else if (!START) {
      alreadyPressedSTART = false;
    }


    if (down && !alreadyPressedDOWN) {
      alreadyPressedDOWN = true;
      set_drible++;
      if (set_drible > 2) set_drible = 0;
    } else if (!down) {
      alreadyPressedDOWN = false;
    }

    if (up && !alreadyPressedUP) {
      alreadyPressedUP = true;
      if (set_drible > 0) set_drible--;
    } else if (!up) {
      alreadyPressedUP = false;
    }

    // === Edit nilai berdasarkan set_drible ===
    if (right && !alreadyPressedRIGHT) {
      alreadyPressedRIGHT = true;

      if (set_drible == 0) {
        Solenoid1 += 10;
      } else if (set_drible == 1) {
        Solenoid2 += 10;
      } else {
        delayTime += 5;
      }
    } else if (!right) {
      alreadyPressedRIGHT = false;
    }

    if (left && !alreadyPressedLEFT) {
      alreadyPressedLEFT = true;

      if (set_drible == 0 && Solenoid1 > 0) {
        Solenoid1 -= 10;
      } else if (set_drible == 1 && Solenoid2 > 0) {
        Solenoid2 -= 10;
      } else if (set_drible == 2 && delayTime > 0) {
        delayTime -= 5;
      }
    } else if (!left) {
      alreadyPressedLEFT = false;
    }


    // if (millis() - lTime3 > 200) {
    //   lTime3 = millis();
    //   blinkState = !blinkState;

    //   if (set_drible == 0) lcd.setCursor(18, 1);
    //   else if (set_drible == 1) lcd.setCursor(18, 2);
    //   else lcd.setCursor(18, 3);
    //   if (blinkState) {
    //     lcd.print("-+");
    //   } else {
    //     lcd.setCursor(18, 1);
    //     lcd.print("  ");
    //     lcd.setCursor(18, 2);
    //     lcd.print("  ");
    //     lcd.setCursor(18, 3);
    //     lcd.print("  ");
    //   }
    // }

    if (millis() - lTime3 > 200) {
      lTime3 = millis();
      blinkState = !blinkState;
      uint8_t row = (set_drible == 0) ? 1 : (set_drible == 1) ? 2 : 3; //tenrary logic
      lcd.setCursor(18, row);
      lcd.print(blinkState ? "-+" : "  ");
    }


    /*print lcd*/
    if (millis() - lTime1 > 200) {
      lTime1 = millis();

      lcd.setCursor(0, 0);
      lcd.print("Set Drible    ");

      lcd.setCursor(0, 1);
      lcd.print("T Solenoid1 : ");
      lcd.print(Solenoid1);
      lcd.print(" ");

      lcd.setCursor(0, 2);
      lcd.print("T Solenoid2 : ");
      lcd.print(Solenoid2);
      lcd.print(" ");

      lcd.setCursor(0, 3);
      lcd.print("Delay Time  : ");
      lcd.print(delayTime);
      lcd.print(" ");
    }
  }
  // sendToSlave();
}

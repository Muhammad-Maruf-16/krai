//set shoot
void setShoot() {
  if (L1) {

    shooter = cross;
    sendToSlave();
    
  } else {
    if (START && !alreadyPressedSTART) {
      alreadyPressedSTART = true;
      M_Shoot_Top1 = 2000;
      M_Shoot_Bottom1 = 2000;
    } else if (!START) {
      alreadyPressedSTART = false;
    }


    if (down && !alreadyPressedDOWN) {
      alreadyPressedDOWN = true;
      set_drible++;
      if (set_drible > 1) set_drible = 0;
    } else if (!down) {
      alreadyPressedDOWN = false;
    }

    if (up && !alreadyPressedUP) {
      alreadyPressedUP = true;
      if (set_drible > 0) set_drible--;
    } else if (!up) {
      alreadyPressedUP = false;
    }

    if (right && !alreadyPressedRIGHT) {
      alreadyPressedRIGHT = true;

      if (set_drible == 0) {
        M_Shoot_Top1 += 50;
      } else {
        M_Shoot_Bottom1 += 50;
      }
    } else if (!right) {
      alreadyPressedRIGHT = false;
    }

    if (left && !alreadyPressedLEFT) {
      alreadyPressedLEFT = true;

      if (set_drible == 0 && M_Shoot_Top1 > 0) {
        M_Shoot_Top1 -= 50;
      } else if (set_drible == 1 && M_Shoot_Bottom1 > 0) {
        M_Shoot_Bottom1 -= 50;
      }
    } else if (!left) {
      alreadyPressedLEFT = false;
    }


    if (millis() - lTime3 > 200) {
      lTime3 = millis();
      blinkState = !blinkState;

      uint8_t row = (set_drible == 0) ? 1 : 2;
      lcd.setCursor(18, row);
      lcd.print(blinkState ? "-+" : "  ");
    }


    /*print lcd*/
    if (millis() - lTime1 > 200) {
      lTime1 = millis();

      lcd.setCursor(0, 0);
      lcd.print("Set Shoot 1  ");

      lcd.setCursor(0, 1);
      lcd.print("M Atas : ");
      lcd.print(M_Shoot_Top1);
      lcd.print(" ");

      lcd.setCursor(0, 2);
      lcd.print("M Bawah : ");
      lcd.print(M_Shoot_Bottom1);
      lcd.print(" ");
    }
  }
}


void setShoot2() {
  if (L1) {

    shooter = cross;
    sendToSlave();
    
  } else {
    if (START && !alreadyPressedSTART) {
      alreadyPressedSTART = true;
      M_Shoot_Top2 = 2500;
      M_Shoot_Bottom2 = 2500;
    } else if (!START) {
      alreadyPressedSTART = false;
    }


    if (down && !alreadyPressedDOWN) {
      alreadyPressedDOWN = true;
      set_drible++;
      if (set_drible > 1) set_drible = 0;
    } else if (!down) {
      alreadyPressedDOWN = false;
    }

    if (up && !alreadyPressedUP) {
      alreadyPressedUP = true;
      if (set_drible > 0) set_drible--;
    } else if (!up) {
      alreadyPressedUP = false;
    }

    if (right && !alreadyPressedRIGHT) {
      alreadyPressedRIGHT = true;

      if (set_drible == 0) {
        M_Shoot_Top2 += 50;
      } else {
        M_Shoot_Bottom2 += 50;
      }
    } else if (!right) {
      alreadyPressedRIGHT = false;
    }

    if (left && !alreadyPressedLEFT) {
      alreadyPressedLEFT = true;

      if (set_drible == 0 && M_Shoot_Top2 > 0) {
        M_Shoot_Top2 -= 50;
      } else if (set_drible == 1 && M_Shoot_Bottom2 > 0) {
        M_Shoot_Bottom2 -= 50;
      }
    } else if (!left) {
      alreadyPressedLEFT = false;
    }


    if (millis() - lTime3 > 200) {
      lTime3 = millis();
      blinkState = !blinkState;

      uint8_t row = (set_drible == 0) ? 1 : 2;
      lcd.setCursor(18, row);
      lcd.print(blinkState ? "-+" : "  ");
    }


    /*print lcd*/
    if (millis() - lTime1 > 200) {
      lTime1 = millis();

      lcd.setCursor(0, 0);
      lcd.print("Set Shoot 2  ");

      lcd.setCursor(0, 1);
      lcd.print("M Atas : ");
      lcd.print(M_Shoot_Top2);
      lcd.print(" ");

      lcd.setCursor(0, 2);
      lcd.print("M Bawah : ");
      lcd.print(M_Shoot_Bottom2);
      lcd.print(" ");
    }
  }
}


void setShoot3() {
  if (L1) {

    shooter = cross;
    sendToSlave();
    
  } else {
    if (START && !alreadyPressedSTART) {
      alreadyPressedSTART = true;
      M_Shoot_Top3 = 3000;
      M_Shoot_Bottom3 = 3000;
    } else if (!START) {
      alreadyPressedSTART = false;
    }


    if (down && !alreadyPressedDOWN) {
      alreadyPressedDOWN = true;
      set_drible++;
      if (set_drible > 1) set_drible = 0;
    } else if (!down) {
      alreadyPressedDOWN = false;
    }

    if (up && !alreadyPressedUP) {
      alreadyPressedUP = true;
      if (set_drible > 0) set_drible--;
    } else if (!up) {
      alreadyPressedUP = false;
    }

    if (right && !alreadyPressedRIGHT) {
      alreadyPressedRIGHT = true;

      if (set_drible == 0) {
        M_Shoot_Top3 += 50;
      } else {
        M_Shoot_Bottom3 += 50;
      }
    } else if (!right) {
      alreadyPressedRIGHT = false;
    }

    if (left && !alreadyPressedLEFT) {
      alreadyPressedLEFT = true;

      if (set_drible == 0 && M_Shoot_Top3 > 0) {
        M_Shoot_Top3 -= 50;
      } else if (set_drible == 1 && M_Shoot_Bottom3 > 0) {
        M_Shoot_Bottom3 -= 50;
      }
    } else if (!left) {
      alreadyPressedLEFT = false;
    }


    if (millis() - lTime3 > 200) {
      lTime3 = millis();
      blinkState = !blinkState;

      uint8_t row = (set_drible == 0) ? 1 : 2;
      lcd.setCursor(18, row);
      lcd.print(blinkState ? "-+" : "  ");
    }


    /*print lcd*/
    if (millis() - lTime1 > 200) {
      lTime1 = millis();

      lcd.setCursor(0, 0);
      lcd.print("Set Shoot 3  ");

      lcd.setCursor(0, 1);
      lcd.print("M Atas : ");
      lcd.print(M_Shoot_Top3);
      lcd.print(" ");

      lcd.setCursor(0, 2);
      lcd.print("M Bawah : ");
      lcd.print(M_Shoot_Bottom3);
      lcd.print(" ");
    }
  }
}



void setShoot4() {
  if (L1) {

    shooter = cross;
    sendToSlave();
    
  } else {
    if (START && !alreadyPressedSTART) {
      alreadyPressedSTART = true;
      M_Shoot_Top4 = 3500;
      M_Shoot_Bottom4 = 3500;
    } else if (!START) {
      alreadyPressedSTART = false;
    }


    if (down && !alreadyPressedDOWN) {
      alreadyPressedDOWN = true;
      set_drible++;
      if (set_drible > 1) set_drible = 0;
    } else if (!down) {
      alreadyPressedDOWN = false;
    }

    if (up && !alreadyPressedUP) {
      alreadyPressedUP = true;
      if (set_drible > 0) set_drible--;
    } else if (!up) {
      alreadyPressedUP = false;
    }

    if (right && !alreadyPressedRIGHT) {
      alreadyPressedRIGHT = true;

      if (set_drible == 0) {
        M_Shoot_Top4 += 50;
      } else {
        M_Shoot_Bottom4 += 50;
      }
    } else if (!right) {
      alreadyPressedRIGHT = false;
    }

    if (left && !alreadyPressedLEFT) {
      alreadyPressedLEFT = true;

      if (set_drible == 0 && M_Shoot_Top4 > 0) {
        M_Shoot_Top4 -= 50;
      } else if (set_drible == 1 && M_Shoot_Bottom4 > 0) {
        M_Shoot_Bottom4 -= 50;
      }
    } else if (!left) {
      alreadyPressedLEFT = false;
    }


    if (millis() - lTime3 > 200) {
      lTime3 = millis();
      blinkState = !blinkState;

      uint8_t row = (set_drible == 0) ? 1 : 2;
      lcd.setCursor(18, row);
      lcd.print(blinkState ? "-+" : "  ");
    }


    /*print lcd*/
    if (millis() - lTime1 > 200) {
      lTime1 = millis();

      lcd.setCursor(0, 0);
      lcd.print("Set Shoot 4  ");

      lcd.setCursor(0, 1);
      lcd.print("M Atas : ");
      lcd.print(M_Shoot_Top4);
      lcd.print(" ");

      lcd.setCursor(0, 2);
      lcd.print("M Bawah : ");
      lcd.print(M_Shoot_Bottom4);
      lcd.print(" ");
    }
  }
}
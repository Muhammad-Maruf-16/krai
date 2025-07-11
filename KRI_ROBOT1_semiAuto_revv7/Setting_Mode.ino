uint8_t setting = 0;
unsigned long lTime2 = 0;
void handleSettingMode() {
  // jika R3 pressed = save & exit
  //jika R3 + L1 = exit
  if (R3) {
    if (L1) {
      //do nothing
    } else {
      prefs.begin("robot_1", false);  // false = read-write mode
      prefs.putInt("deOffset", deOffset);
      prefs.putInt("beOffset", beOffset);
      prefs.putInt("kaOffset", kaOffset);
      prefs.putInt("kiOffset", kiOffset);
      prefs.putInt("Solenoid1", Solenoid1);
      prefs.putInt("Solenoid2", Solenoid2);
      prefs.putInt("delayTime", delayTime);
      prefs.putInt("M1", M1);
      prefs.putInt("M2", M2);
      prefs.putInt("M3", M3);
      prefs.putInt("M4", M4);
      prefs.putInt("slowRote", slowRote);
      prefs.putInt("M_Shoot_Top1", M_Shoot_Top1);
      prefs.putInt("M_Shoot_Bottom1", M_Shoot_Bottom1);
      prefs.putInt("M_Shoot_Top2", M_Shoot_Top2);
      prefs.putInt("M_Shoot_Bottom2", M_Shoot_Bottom2);
      prefs.putInt("M_Shoot_Top3", M_Shoot_Top3);
      prefs.putInt("M_Shoot_Bottom3", M_Shoot_Bottom3);
      prefs.putInt("M_Shoot_Top4", M_Shoot_Top4);
      prefs.putInt("M_Shoot_Bottom4", M_Shoot_Bottom4);
      prefs.end();
    }
    lcd.clear();
    lcd.print("SPEED ");
    lcd.print(DPAD_SPEED);
    settingMode = false;
  }

  //select setting
  if (L3 && !alreadyPressedL3) {
    alreadyPressedL3 = true;
    if (L1) {
      if (setting > 0) setting--;
      lcd.clear();
    } else {
      if (setting < 6) setting++;
      lcd.clear();
    }
  } else if (!L3) {
    alreadyPressedL3 = false;
  }

  static bool printed = false;
  if (setting == 0) settingRPM();
  else if (setting == 1) setBelok();
  else if (setting == 2) setDrible();
  else if (setting == 3) setShoot();
  else if (setting == 4) setShoot2();
  else if (setting == 5) setShoot3();
  else if (setting == 6) setShoot4();
  // else if (setting == 3) tesDrible();
  // else if (setting == 3) setRobot();
}
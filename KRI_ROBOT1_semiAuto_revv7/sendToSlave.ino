void sendToSlave() { //kirim ke slave
  char buf[64];
  sprintf(buf, "A%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n", deRPM, beRPM, kaRPM, kiRPM, sol1, sol2, shooterPos, shooter, drible_arm, loss, M_Shoot_Top, M_Shoot_Bottom, rawBost, autoModeFlag, maxMode);

  // debugEncoder();  // Print tick encoder
  // debugGyroCorrection();
  // Serial.print(buf);
  Serial2.print(buf);
}
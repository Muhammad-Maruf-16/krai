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
    // If no direction button pressed, show current offsets
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
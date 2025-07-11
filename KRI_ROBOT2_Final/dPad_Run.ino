
void calculateDpadMovement() {
  deRPM = beRPM = kaRPM = kiRPM = 0;



  if (up && !down || stickLY < 30) {
    if (left && !right) {
      deRPM = DPAD_SPEED * 2 - deOffset;
      beRPM = -DPAD_SPEED * 2 + beOffset;
      kaRPM = 0;
      kiRPM = 0;
    } else if (right && !left) {
      deRPM = 0;
      beRPM = 0;
      kaRPM = -DPAD_SPEED * 2 + kaOffset;
      kiRPM = DPAD_SPEED * 2 - kiOffset;
    } else if (stickRX < 30) {
      deRPM = DPAD_SPEED + M3;
      beRPM = -DPAD_SPEED - M1;
      kaRPM = -DPAD_SPEED - M2;
      kiRPM = DPAD_SPEED + M4;
    } else if (stickRX > 230) {
      deRPM = DPAD_SPEED + M4;
      beRPM = -DPAD_SPEED - M2;
      kaRPM = -DPAD_SPEED - M1;
      kiRPM = DPAD_SPEED + M3;
    } else {
      deRPM = DPAD_SPEED - deOffset;
      beRPM = -DPAD_SPEED + beOffset;
      kaRPM = -DPAD_SPEED + kaOffset;
      kiRPM = DPAD_SPEED - kiOffset;
    }
    if (stickRY > 230) {
      deRPM = DPAD_SPEED + M3B;
      beRPM = -DPAD_SPEED - M1B;
      kaRPM = -DPAD_SPEED - M2B;
      kiRPM = DPAD_SPEED + M4B;
    } else if (stickRY < 30) {
      deRPM = DPAD_SPEED + M4B;
      beRPM = -DPAD_SPEED - M2B;
      kaRPM = -DPAD_SPEED - M1B;
      kiRPM = DPAD_SPEED + M3B;
    }
    // if (L2) {

    //   deRPM = 0;
    //   beRPM =0;
    //   kaRPM =0;
    //   kiRPM = 0;
    // }
  } else if (down && !up || stickLY > 230) {
    if (left && !right) {
      deRPM = 0;
      beRPM = 0;
      kaRPM = DPAD_SPEED * 2 - kaOffset;
      kiRPM = -DPAD_SPEED * 2 + kiOffset;
    } else if (right && !left) {
      deRPM = -DPAD_SPEED * 2 + deOffset;
      beRPM = DPAD_SPEED * 2 - beOffset;
      kaRPM = 0;
      kiRPM = 0;
    } else if (stickRX < 30) {
      deRPM = -DPAD_SPEED - M2;
      beRPM = DPAD_SPEED + M4;
      kaRPM = DPAD_SPEED + M3;
      kiRPM = -DPAD_SPEED - M1;
    } else if (stickRX > 230) {
      deRPM = -DPAD_SPEED - M1;
      beRPM = DPAD_SPEED + M3;
      kaRPM = DPAD_SPEED + M4;
      kiRPM = -DPAD_SPEED - M2;
    } else {
      deRPM = -DPAD_SPEED + deOffset;
      beRPM = DPAD_SPEED - beOffset;
      kaRPM = DPAD_SPEED - kaOffset;
      kiRPM = -DPAD_SPEED + kiOffset;
    }
    if (stickRY > 230) {
      deRPM = -DPAD_SPEED - M2B;
      beRPM = DPAD_SPEED + M4B;
      kaRPM = DPAD_SPEED + M3B;
      kiRPM = -DPAD_SPEED - M1B;
    } else if (stickRY < 30) {
      deRPM = -DPAD_SPEED - M1B;
      beRPM = DPAD_SPEED + M3B;
      kaRPM = DPAD_SPEED + M4B;
      kiRPM = -DPAD_SPEED - M2B;
    }
    // if (L2) {

    //   deRPM =0;
    //   beRPM = 0;
    //   kaRPM = 0;
    //   kiRPM =0;
    // }
  } else if (left && !right) {
    if (stickRX < 30) {
      beRPM = -DPAD_SPEED - M4;
      kaRPM = DPAD_SPEED + M1;
      deRPM = DPAD_SPEED + M2;
      kiRPM = -DPAD_SPEED - M3;
    } else if (stickRX > 230) {
      beRPM = -DPAD_SPEED - M3;
      kaRPM = DPAD_SPEED + M2;
      deRPM = DPAD_SPEED + M1;
      kiRPM = -DPAD_SPEED - M4;
    } else {
      deRPM = DPAD_SPEED - deOffset;
      beRPM = -DPAD_SPEED + beOffset;
      kaRPM = DPAD_SPEED - kaOffset;
      kiRPM = -DPAD_SPEED + kiOffset;
    }
    if (stickRY > 230) {
      beRPM = -DPAD_SPEED - M4B;
      kaRPM = DPAD_SPEED + M1B;
      deRPM = DPAD_SPEED + M2B;
      kiRPM = -DPAD_SPEED - M3B;
    } else if (stickRY < 30) {
      beRPM = -DPAD_SPEED - M3B;
      kaRPM = DPAD_SPEED + M2B;
      deRPM = DPAD_SPEED + M1B;
      kiRPM = -DPAD_SPEED - M4B;
    }
    // if (L2) {
    //   deRPM = 0;
    //   beRPM =0;
    //   kaRPM = 0;
    //   kiRPM =0;
    // }
  } else if (right && !left) {
    if (stickRX < 30) {
      beRPM = DPAD_SPEED + M2;
      kaRPM = -DPAD_SPEED - M3;
      deRPM = -DPAD_SPEED - M4;
      kiRPM = DPAD_SPEED + M1;
    } else if (stickRX > 230) {
      beRPM = DPAD_SPEED + M1;
      kaRPM = -DPAD_SPEED - M4;
      deRPM = -DPAD_SPEED - M3;
      kiRPM = DPAD_SPEED + M2;
    } else {
      deRPM = -DPAD_SPEED + deOffset;
      beRPM = DPAD_SPEED - beOffset;
      kaRPM = -DPAD_SPEED + kaOffset;
      kiRPM = DPAD_SPEED - kiOffset;
    }
    if (stickRY > 230) {
      beRPM = DPAD_SPEED + M2B;
      kaRPM = -DPAD_SPEED - M3B;
      deRPM = -DPAD_SPEED - M4B;
      kiRPM = DPAD_SPEED + M1B;
    } else if (stickRY < 30) {
      beRPM = DPAD_SPEED + M1B;
      kaRPM = -DPAD_SPEED - M4B;
      deRPM = -DPAD_SPEED - M3B;
      kiRPM = DPAD_SPEED + M2B;
    }
    // if (L2) {
    //   deRPM =0;
    //   beRPM = 0;
    //   kaRPM =0;
    //   kiRPM = 0;
    // }
  } else if (stickRY > 230) {
    deRPM = -slowRoteB + deOffset;
    beRPM = -slowRoteB + beOffset;
    kaRPM = slowRoteB - kaOffset;
    kiRPM = slowRoteB - kiOffset;
  } else if (stickRY < 30) {
    deRPM = slowRoteB - deOffset;
    beRPM = slowRoteB - beOffset;
    kaRPM = -slowRoteB + kaOffset;
    kiRPM = -slowRoteB + kiOffset;
  } else if (stickRX < 30) {
    deRPM = -slowRote + deOffset;
    beRPM = -slowRote + beOffset;
    kaRPM = slowRote - kaOffset;
    kiRPM = slowRote - kiOffset;
  } else if (stickRX > 230) {
    deRPM = slowRote - deOffset;
    beRPM = slowRote - beOffset;
    kaRPM = -slowRote + kaOffset;
    kiRPM = -slowRote + kiOffset;
  }
  if (stickLX < 30) {
    deRPM = DPAD_SPEED - deOffset;
    beRPM = -DPAD_SPEED + beOffset;
    kaRPM = DPAD_SPEED - kaOffset;
    kiRPM = -DPAD_SPEED + kiOffset;
  } else if (stickLX > 230) {
    deRPM = -DPAD_SPEED + deOffset;
    beRPM = DPAD_SPEED - beOffset;
    kaRPM = -DPAD_SPEED + kaOffset;
    kiRPM = DPAD_SPEED - kiOffset;
  }

  if (L2) {
    deRPM = 0;
    beRPM = 0;
    kaRPM = 0;
    kiRPM = 0;
    loss = 1;
  }
  // if (L2) loss = 1;
  else
    loss = 0;
  normalizePWM();
  if (L1) {
    deRPM *= 2;
    beRPM *= 2;
    kaRPM *= 2;
    kiRPM *= 2;
  }
}

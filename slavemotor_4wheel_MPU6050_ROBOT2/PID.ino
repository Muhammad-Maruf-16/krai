
//program untuk mengatur rpm motor dengan PID
void RPMdepan(int rpm, float kp, float ki, float kd) {
  float PID, P, I, D;
  float Error, lError, cumError, rateError;
  float setRPM, RPM;
  int PULSE, TS = 10;

  setRPM = rpm;
  if ((millis() - ltime) >= 10) {
    ltime = millis();
    // noInterrupts();
    PULSE = ENC_DE_COUNT;
    ENC_DE_COUNT = 0;
    // interrupts();
    RPM = float((PULSE * 3000.0) / REVOLUTIONA);

    if (setRPM > 0) {
      DE_DIRECTION(CW);
    } else if (setRPM < 0) {
      DE_DIRECTION(CCW);
    } else if (setRPM == 0) {
      DE_DIRECTION(Stop);
      PWM_DE = 0;
    }

    // output P
    Error = abs(setRPM) - abs(RPM);
    P = kp * Error;
    // output I
    cumError += Error * TS;
    I = ki * cumError;
    // output D
    rateError = (Error - lError) / TS;
    D = kd * rateError;
    PID = P + I + D;
    // PID = constrain(PID, 0, 255);
    PWM_DE = PWM_DE + PID;
    lError = Error;


    if (PWM_DE > 255) PWM_DE = 255;
    else if (PWM_DE < 0) PWM_DE = 0;

    // if (rpm!=0) Serial.println(RPM);
    // Serial.print(RPM);
    // Serial.print("\t");
  }
}




void RPMbelakang(int rpm, float kp, float ki, float kd) {
  float PID, P, I, D;
  float Error, lError, cumError, rateError;
  float setRPM, RPMB;
  int PULSE1, TS = 10;



  setRPM = rpm;
  if ((millis() - ltime1) >= 10) {
    ltime1 = millis();

    PULSE1 = ENC_BE_COUNT;
    ENC_BE_COUNT = 0;

    RPMB = float((PULSE1 * 3000.0) / REVOLUTIONA);

    if (setRPM > 0) {
      BE_DIRECTION(CW);
    } else if (setRPM < 0) {
      BE_DIRECTION(CCW);
    } else if (setRPM == 0) {
      BE_DIRECTION(Stop);
      PWM_BE = 0;
    }

    // output P
    Error = abs(setRPM) - abs(RPMB);
    P = kp * Error;
    // output I
    cumError += Error * TS;
    I = ki * cumError;
    // output D
    rateError = (Error - lError) / TS;
    D = kd * rateError;
    PID = P + I + D;
    PWM_BE = PWM_BE + PID;
    lError = Error;


    if (PWM_BE > 255) PWM_BE = 255;
    else if (PWM_BE < 0) PWM_BE = 0;

    // Serial.print(-RPMB);
    // Serial.print("\t");
  }
}

void RPMkanan(int rpm, float kp, float ki, float kd) {
  float PID, P, I, D;
  float Error, lError, cumError, rateError;
  float setRPM, RPMC;
  int PULSE, TS = 10;
  setRPM = rpm;
  if ((millis() - ltime2) >= 10) {
    ltime2 = millis();

    PULSE = ENC_KA_COUNT;
    ENC_KA_COUNT = 0;

    RPMC = float((PULSE * 3000.0) / REVOLUTIONA);

    if (setRPM > 0) {
      KA_DIRECTION(CW);
    } else if (setRPM < 0) {
      KA_DIRECTION(CCW);
    } else if (setRPM == 0) {
      KA_DIRECTION(Stop);
      PWM_KA = 0;
    }

    // output P
    Error = abs(setRPM) - abs(RPMC);
    P = kp * Error;
    // output I
    cumError += Error * TS;
    I = ki * cumError;
    // output D
    rateError = (Error - lError) / TS;
    D = kd * rateError;
    PID = P + I + D;
    PWM_KA = PWM_KA + PID;
    lError = Error;

    if (PWM_KA > 255) PWM_KA = 255;
    else if (PWM_KA < 0) PWM_KA = 0;

    // Serial.print(RPMC);
    // Serial.print("\t");
  }
}

void RPMkiri(int rpm, float kp, float ki, float kd) {
  float PID, P, I, D;
  float Error, lError, cumError, rateError;
  float setRPM, RPMD;
  int PULSE, TS = 10;

  setRPM = rpm;
  if ((millis() - ltime3) >= 10) {
    ltime3 = millis();

    PULSE = ENC_KI_COUNT;
    ENC_KI_COUNT = 0;

    RPMD = float((PULSE * 3000.0) / REVOLUTIONA);

    if (setRPM > 0) {
      KI_DIRECTION(CW);
    } else if (setRPM < 0) {
      KI_DIRECTION(CCW);
    } else if (setRPM == 0) {
      KI_DIRECTION(Stop);
      PWM_KI = 0;
    }

    // output P
    Error = abs(setRPM) - abs(RPMD);
    P = kp * Error;
    // output I
    cumError += Error * TS;
    I = ki * cumError;
    // output D
    rateError = (Error - lError) / TS;
    D = kd * rateError;
    PID = P + I + D;
    PWM_KI = PWM_KI + PID;
    lError = Error;

    if (PWM_KI > 255) PWM_KI = 255;
    else if (PWM_KI < 0) PWM_KI = 0;

    // Serial.print(-RPMD);
    // Serial.println("\t");
  }
}

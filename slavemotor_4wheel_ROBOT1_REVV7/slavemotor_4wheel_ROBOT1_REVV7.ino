
// #include <digitalWriteFast.h>
int tes = 0, tes1 = 0;

#define CW 1
#define CCW 2
#define Stop 3

/* AT COMMAND */
#define AT_RESET "RESET"
#define AT_RPS "RPS"

/*COM*/
#define slaveSerial Serial2
#define espSerial Serial1
#define REVOLUTIONB 94.0   //pg45 50KE7
#define REVOLUTIONA 134.0  //pg45 19.2K

/*encoder*/
/*shield lama*/
// #define ENC_DE_CHA 23
// #define ENC_DE_CHB 25
// #define ENC_BE_CHA 27
// #define ENC_BE_CHB 29
// #define ENC_KI_CHA 31
// #define ENC_KI_CHB 33
// #define ENC_KA_CHA 35
// #define ENC_KA_CHB 37
/*shield v2*/
#define ENC_DE_CHA 27
#define ENC_DE_CHB 29
#define ENC_BE_CHA 31
#define ENC_BE_CHB 33
#define ENC_KA_CHA 35
#define ENC_KA_CHB 37
#define ENC_KI_CHA 41
#define ENC_KI_CHB 39

#define ENC_X_CHA 43
#define ENC_X_CHB 45
#define ENC_Y_CHA 47
#define ENC_Y_CHB 49

#define ENC_SHOOT_TOP_CHA 51
#define ENC_SHOOT_TOP_CHB 53
#define ENC_SHOOT_BOTTOM_CHA 69
#define ENC_SHOOT_BOTTOM_CHB 68

/*motor base*/
#define DE_RPWM_PIN 4
#define DE_LPWM_PIN 5
#define BE_RPWM_PIN 8
#define BE_LPWM_PIN 9
#define KI_RPWM_PIN 2
#define KI_LPWM_PIN 3
#define KA_RPWM_PIN 6
#define KA_LPWM_PIN 7


// #define DE_RPWM_PIN 6
// #define DE_LPWM_PIN 7
// #define BE_RPWM_PIN 2
// #define BE_LPWM_PIN 3
// #define KI_RPWM_PIN 4
// #define KI_LPWM_PIN 5
// #define KA_RPWM_PIN 8
// #define KA_LPWM_PIN 9

#define pwmShoot1 255
#define pwmShoot2 255
/*motor shooter*/
#define M_Shooter1_PIN 12
#define M_Shooter2_PIN 13

/*solenoid*/
#define sol_Sudut_Up_PIN 26
#define sol_Sudut_Down_PIN 28
#define sol2Up 32
#define sol2Down 30
#define sol_Shooter_Up_PIN 34
#define sol_Shooter_Down_PIN 44
#define sol1Up 46
#define sol1Down 48
#define sol_Arm_Up 50
#define sol_Arm_Down 52

#define WHEEL_RADIUS 0.075  // Radius roda
const float linearVelocity = 2.0;


//deklarasi invers kinematic
float w1,
  w2,
  w3,
  v1,
  v2,
  v3;
/*PULSE*/
volatile int32_t
  ENC_DE_COUNT = 0,
  ENC_BE_COUNT = 0,
  ENC_KA_COUNT = 0,
  ENC_KI_COUNT = 0,
  ENC_X_COUNT = 0,
  ENC_Y_COUNT = 0,
  ENC_SHOOT_TOP_COUNT = 0,
  ENC_SHOOT_BOTTOM_COUNT = 0;
volatile long int
  LENC_DE_COUNT = 0,
  LENC_BE_COUNT = 0,
  LENC_KA_COUNT = 0,
  LENC_KI_COUNT = 0;
float
  PWM_DE = 0,
  PWM_BE = 0,
  PWM_KA = 0,
  PWM_KI = 0,
  PWM_SHOOT_TOP = 0,
  PWM_SHOOT_BOTTOM = 0,
  RPME,
  RPMF,
  aaa = 500, 
  bbb = 0.1, 
  ccc = 0.0, 
  ddd = 0.0; 
unsigned long
  ltime,
  ltime1,
  ltime2,
  ltime3,
  lTime4,
  lTime5,
  ltime6,
  ltime7,
  lTime11 = 0,
  Previousmillis;
int
  lastPulses = 0,
  M_Shooter1_PWM = 2000,
  M_Shooter2_PWM = 2000;
bool
  status_ARM = false,
  direction_ARM = false,
  last_dribleARM = false,
  status_Shoot = false,
  status_Shoot1 = false,
  autoMode = false,
  manualMode = false;
static bool
  lastDownState = 0;




// Global variables for soft stop
unsigned long previousMillis = 0;
const long interval = 80;
int targetDeRPM = 0;
int targetBeRPM = 0;
int targetKaRPM = 0;
int targetKiRPM = 0;
int currentDeRPM = 0;
int currentBeRPM = 0;
int currentKaRPM = 0;
int currentKiRPM = 0;

void applySoftStop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    if (currentDeRPM != 0) {
      currentDeRPM = (abs(currentDeRPM) > 10) ? currentDeRPM * 0.7 : 0;
    }
    if (currentBeRPM != 0) {
      currentBeRPM = (abs(currentBeRPM) > 10) ? currentBeRPM * 0.7 : 0;
    }
    if (currentKaRPM != 0) {
      currentKaRPM = (abs(currentKaRPM) > 10) ? currentKaRPM * 0.7 : 0;
    }
    if (currentKiRPM != 0) {
      currentKiRPM = (abs(currentKiRPM) > 10) ? currentKiRPM * 0.7 : 0;
    }
  }
}



int setRpm = 600;
int currentRpm = 300;
unsigned long lastTime1 = 0;
unsigned long modeStartTime = 0;
int mode = 0;
void coba() {
  switch (mode) {
    case 0:
      Serial.println("1");
      currentRpm = setRpm;
      if (modeStartTime == 0) modeStartTime = millis();
      if (millis() - modeStartTime >= 100) {
        mode = 1;
        lastTime1 = 0;
      }
      break;
    case 1:
      Serial.println("2");
      if (lastTime1 == 0) lastTime1 = millis();
      if (millis() - lastTime1 >= 20 && currentRpm > 0) {
        lastTime1 = millis();
        currentRpm = max(0, currentRpm - 2);
      }
      if (currentRpm == 0) {
        setRpm = -600;
        currentRpm = setRpm;
        mode = 2;
        modeStartTime = millis();
      }
      break;
    case 2:
      currentRpm = setRpm;
      Serial.println("3");
      if (millis() - modeStartTime >= 90) {
        mode = 4;
        lastTime1 = 0;
      }
      break;
    case 3:
      if (lastTime1 == 0) lastTime1 = millis();
      if (millis() - lastTime1 >= 20 && currentRpm < 0) {
        lastTime1 = millis();
        currentRpm = min(0, currentRpm + 2);
      }
      Serial.println("4");
      if (currentRpm == 0) {
        mode = 4;  // Selesai
      }
      break;
    case 4:
      currentRpm = 0;
      break;
  }
  // Update motor, dengan RPMkiri mendapatkan nilai negatif dari currentRpm
  RPMdepan(-currentRpm, 0.05, 0.0002, 0.5);
  RPMbelakang(currentRpm, 0.05, 0.0002, 0.5);
  RPMkanan(currentRpm, 0.05, 0.0002, 0.5);
  RPMkiri(currentRpm, 0.06, 0.0002, 0.5);
}



void trialPID() {
  //MAKSIMAL 650
  if (millis() - lTime4 >= 5000) {
    RPMdepan(0, 0, 0, 0);
    RPMbelakang(0, 0, 0, 0);
    RPMkanan(0, 0, 0, 0);
    RPMkiri(0, 0, 0, 0);
  } else {
    RPMdepan(setRpm, 0.05, 0.0002, 0.5);
    RPMbelakang(setRpm, 0.05, 0.0002, 0.5);
    RPMkanan(setRpm, 0.05, 0.0002, 0.5);
    RPMkiri(-setRpm, 0.05, 0.0002, 0.45);
  }
  // } else {
  //   RPMdepan(aaa, bbb, ccc, ddd);
  //   RPMbelakang(aaa, bbb, ccc, ddd);
  //   RPMkanan(aaa, bbb, ccc, ddd);
  //   RPMkiri(aaa, bbb, ccc, ddd);
  // }
  //RPMbelakang(200, 0.1, 0.0001, 0.001);
  // RPMkanan(100, 0.12, 0.0, 0.06);
  // RPMkiri(100, 0.115, 0.0, 0.0);
}


//program untuk rotary encoder
void ENC_DE_FUNCT() {
  digitalRead(ENC_DE_CHB) ? ENC_DE_COUNT-- : ENC_DE_COUNT++;
}

void ENC_BE_FUNCT() {
  digitalRead(ENC_BE_CHB) ? ENC_BE_COUNT-- : ENC_BE_COUNT++;
}

void ENC_KA_FUNCT() {
  digitalRead(ENC_KA_CHB) ? ENC_KA_COUNT-- : ENC_KA_COUNT++;
}

void ENC_KI_FUNCT() {
  digitalRead(ENC_KI_CHB) ? ENC_KI_COUNT-- : ENC_KI_COUNT++;
}

void ENC_X_FUNCT() {
  digitalRead(ENC_X_CHB) ? ENC_X_COUNT-- : ENC_X_COUNT++;
}

void ENC_Y_FUNCT() {
  digitalRead(ENC_Y_CHB) ? ENC_Y_COUNT-- : ENC_Y_COUNT++;
}

void ENC_SHOOT_TOP_FUNCT(){
  digitalRead(ENC_SHOOT_TOP_CHB) ? ENC_SHOOT_TOP_COUNT-- : ENC_SHOOT_TOP_COUNT++;
}

void ENC_SHOOT_BOTTOM_FUNCT(){
  digitalRead(ENC_SHOOT_BOTTOM_CHB) ? ENC_SHOOT_BOTTOM_COUNT-- : ENC_SHOOT_BOTTOM_COUNT++;
}

/*cek resolusi encoder berapa sinyal per putaran*/
void readENC() {
  Serial.print(ENC_SHOOT_BOTTOM_COUNT);  //94
  Serial.print("  ");
  Serial.print(ENC_SHOOT_TOP_COUNT);  //134
  Serial.print("  ");
  Serial.print(ENC_X_COUNT);  //134
  Serial.print("  ");
  Serial.print(ENC_Y_COUNT);  //94
  Serial.print("  ");
  Serial.print(ENC_DE_COUNT);  //94
  Serial.print("  ");
  Serial.print(ENC_BE_COUNT);  //134
  Serial.print("  ");
  Serial.print(ENC_KA_COUNT);  //134
  Serial.print("  ");
  Serial.print(ENC_KI_COUNT);  //94
  Serial.println("  ");
}

/*coba cek rpm*/
void tesRPM() {
  analogWrite(M_Shooter2_PIN, 50);
  if (millis() - lTime4 >= 1000) {
    noInterrupts();
    int32_t pulses = ENC_SHOOT_BOTTOM_COUNT;
    ENC_SHOOT_BOTTOM_COUNT = 0;
    interrupts();
    int32_t rpmtes = (pulses / 600) * 60;
    int32_t selisih = lastPulses - pulses;
    Serial.print(rpmtes);
    Serial.print("  ");
    Serial.println(selisih);
    // lastPulses = pulses;
    lTime4 = millis();
  }
}


/*cek input serial ini digunakan pas uji coba tuning PID manualan*/
void checkSerialInput() {
  if (Serial.available() > 0) {
    char input[30];
    int len = Serial.readBytesUntil('\n', input, sizeof(input) - 1);
    input[len] = '\0';
    if (input[0] == '#') {  // Pastikan input diawali dengan #
      float tempA, tempB, tempC, tempD;
      if (sscanf(input + 1, "%f,%f,%f,%f", &tempA, &tempB, &tempC, &tempD) == 4) {
        aaa = tempA;
        bbb = tempB;
        ccc = tempC;
        ddd = tempD;
        Serial.print("Updated values: ");
        Serial.print(aaa);
        Serial.print(", ");
        Serial.print(bbb, 5);
        Serial.print(", ");
        Serial.print(ccc, 5);
        Serial.print(", ");
        Serial.println(ddd, 5);
        lTime4 = millis();
      }
    }
  }
}

/*program untuk percobaan membaca rpm*/

void readRPM(int Speed) {
  float RPM, RPM1;
  int PULSE;

  analogWrite(DE_RPWM_PIN, Speed);
  digitalWrite(DE_LPWM_PIN, LOW);

  if ((millis() - Previousmillis) >= 1000) {
    Previousmillis = millis();
    noInterrupts();
    PULSE = ENC_DE_COUNT;
    ENC_DE_COUNT = 0;
    interrupts();
    RPM = float((PULSE * 60.0) / 134);
    RPM1 = float((PULSE / 134.0) * 60);
    Serial.print(RPM);
    Serial.print("  ");
    Serial.println(RPM1);
    // ENC_DE_COUNT = 0;
  }
}
/* dengan pembacaan setiap 1s
depan 271 460
belakang 233 560
kanan 222
kiri 259

pembacaan setiap 0.1s
gak stabil
*/

//program untuk mencoba encoder
void trialEnc() {
  while (1) {

    Serial.print("DE_ENC: ");
    Serial.print(ENC_DE_COUNT);
    Serial.print("\t");

    Serial.print("BE_ENC: ");
    Serial.print(ENC_BE_COUNT);
    Serial.print("\t");

    Serial.print("KA_ENC: ");
    Serial.print(ENC_KA_COUNT);
    Serial.print("\t");

    Serial.print("KI_ENC: ");
    Serial.println(ENC_KI_COUNT);
    delay(100);
  }
}


//program untuk mencoba pwm
void trialPWM(int Speed) {
  analogWrite(BE_RPWM_PIN, Speed);
  digitalWrite(BE_LPWM_PIN, LOW);
  analogWrite(KA_RPWM_PIN, Speed);
  digitalWrite(KA_LPWM_PIN, LOW);
  analogWrite(KI_RPWM_PIN, Speed);
  digitalWrite(KI_LPWM_PIN, LOW);
  // delay(1000);
  // analogWrite(BE_LPWM_PIN,  Speed);
  // digitalWrite(BE_RPWM_PIN, LOW);
  // analogWrite(KA_LPWM_PIN,  Speed);
  // digitalWrite(KA_RPWM_PIN, LOW);
  // analogWrite(KI_LPWM_PIN,  Speed);
  // digitalWrite(KI_RPWM_PIN, LOW);
  // delay(1000);
}



//program untuk mengatur arah gerakan motor
void DE_DIRECTION(int dir) {
  switch (dir) {
    case CW:
      analogWrite(DE_RPWM_PIN, round(PWM_DE));
      digitalWrite(DE_LPWM_PIN, LOW);
      break;
    case CCW:
      digitalWrite(DE_RPWM_PIN, LOW);
      analogWrite(DE_LPWM_PIN, round(PWM_DE));
      break;
    case Stop:
      digitalWrite(DE_RPWM_PIN, LOW);
      digitalWrite(DE_LPWM_PIN, LOW);
      break;
  }
}
void BE_DIRECTION(int dir) {
  switch (dir) {
    case CW:
      analogWrite(BE_RPWM_PIN, round(PWM_BE));
      digitalWrite(BE_LPWM_PIN, LOW);
      break;
    case CCW:
      digitalWrite(BE_RPWM_PIN, LOW);
      analogWrite(BE_LPWM_PIN, round(PWM_BE));
      break;
    case Stop:
      digitalWrite(BE_RPWM_PIN, LOW);
      digitalWrite(BE_LPWM_PIN, LOW);
      break;
  }
}
void KA_DIRECTION(int dir) {
  switch (dir) {
    case CW:
      analogWrite(KA_RPWM_PIN, round(PWM_KA));
      digitalWrite(KA_LPWM_PIN, LOW);
      break;
    case CCW:
      digitalWrite(KA_RPWM_PIN, LOW);
      analogWrite(KA_LPWM_PIN, round(PWM_KA));
      break;
    case Stop:
      digitalWrite(KA_RPWM_PIN, LOW);
      digitalWrite(KA_LPWM_PIN, LOW);
      break;
  }
}
void KI_DIRECTION(int dir) {
  switch (dir) {
    case CW:
      analogWrite(KI_RPWM_PIN, round(PWM_KI));
      digitalWrite(KI_LPWM_PIN, LOW);
      break;
    case CCW:
      digitalWrite(KI_RPWM_PIN, LOW);
      analogWrite(KI_LPWM_PIN, round(PWM_KI));
      break;
    case Stop:
      digitalWrite(KI_RPWM_PIN, LOW);
      digitalWrite(KI_LPWM_PIN, LOW);
      break;
  }
}

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
    PWM_DE = PWM_DE + PID;
    lError = Error;

    if (PWM_DE > 255) PWM_DE = 255;
    else if (PWM_DE < 0) PWM_DE = 0;

    // Optional: kurangi frekuensi serial print untuk menghindari bottleneck
    // static int printCounter = 0;
    // if (printCounter++ >= 10) { // Print setiap 10ms (10 x 10ms)
    // SerialUSB.print(rpm);
    // SerialUSB.print("\t");
    // SerialUSB.print(RPM);
    // SerialUSB.print("\t");
    // Serial.print(PWM_DE);
    // Serial.println("\t");
    // printCounter = 0;
    // }
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

    RPMB = float((PULSE1 * 3000) / REVOLUTIONA);

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

    //SerialUSB.print(rpm);
    // SerialUSB.print("\t");
    // SerialUSB.print(-RPMB);
    // SerialUSB.println("\t");
    // Serial.print(rpm);
    // Serial.print("\t");
    // Serial.print(-RPMB);
    // Serial.print("\t");
    // Serial.print(PID);
    // Serial.println("\t");

    // Serial.print(PWM_BE);
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

    RPMC = float((PULSE * 3000) / REVOLUTIONA);

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

    // Serial.print("\t");
    // Serial.print(RPMC);

    // Serial.print(PWM_KA);
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

    RPMD = float((PULSE * 3000) / REVOLUTIONA);

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


    // Serial.print("\t");
    // Serial.print(RPMD);
    // Serial.print("\t");
    // Serial.print(I);
    // Serial.println("\t");

    // Serial.print(PWM_KI);
    // Serial.println("\t");
  }
}


void RPMShootTop(int rpm, float kp, float ki, float kd, float factor) {
  static float PID, P, I, D;
  static float Error, lError, cumError, rateError;
  static float setRPM;
  static unsigned long startTime = 0;
  static bool firstRun = true;

  const int TS = 10;

  if (firstRun && rpm > 0) {
    startTime = millis();
    firstRun = false;
  } else if (rpm == 0) {
    // Reset state jika berhenti
    cumError = 0;
    lError = 0;
    firstRun = true;
    PWM_SHOOT_TOP = 0;
    digitalWrite(M_Shooter1_PIN, LOW);
    return;
  }

  if ((millis() - ltime6) >= TS) {
    ltime6 = millis();


    int PULSE = ENC_SHOOT_TOP_COUNT;
    ENC_SHOOT_TOP_COUNT = 0;

    RPME = float((PULSE * 3000.0) / 600.0);

    Error = rpm - RPME;
    P = kp * Error;
    cumError += Error * TS;
    I = ki * cumError;
    rateError = (Error - lError) / TS;
    D = kd * rateError;
    PID = P + I + D;
    lError = Error;

    unsigned long elapsed = millis() - startTime;
    int targetPWM = map(rpm, 0, 7000, 0, 235);
    int rampPWM = map(elapsed, 0, 2000, 0, targetPWM);  // Feedforward
    if (rampPWM > targetPWM) rampPWM = targetPWM;
    if (rampPWM > 255) rampPWM = 255;


    // Gabungkan PWM ramp + PID
    if (rpm - RPME <= 5) {
      PWM_SHOOT_TOP += PID;
    } else {
      PWM_SHOOT_TOP = rampPWM + factor * PID;
    }

    // Batas PWM
    if (PWM_SHOOT_TOP > 255) PWM_SHOOT_TOP = 255;
    else if (PWM_SHOOT_TOP < 0) PWM_SHOOT_TOP = 0;

    // Output PWM
    analogWrite(M_Shooter1_PIN, PWM_SHOOT_TOP);

    // SerialUSB.print(RPME);
    // SerialUSB.print("\t");
    // SerialUSB.print(rampPWM);
    // SerialUSB.print("\t");
    // SerialUSB.print(PID);
    // SerialUSB.print("\t");
    // SerialUSB.print(PWM_SHOOT_TOP);
    // SerialUSB.print("\t");
  }
}

void RPMShootBottom(int rpm, float kp, float ki, float kd, float factor) {
  static float PID, P, I, D;
  static float Error, lError, cumError, rateError;
  static float setRPM;
  static unsigned long startTime = 0;
  static bool firstRun = true;

  const int TS = 10;


  if (firstRun && rpm > 0) {
    startTime = millis();
    firstRun = false;
  } else if (rpm == 0) {
    // Reset state jika berhenti
    cumError = 0;
    lError = 0;
    firstRun = true;
    PWM_SHOOT_BOTTOM = 0;
    digitalWrite(M_Shooter2_PIN, LOW);
    return;
  }

  if ((millis() - ltime7) >= TS) {
    ltime7 = millis();

    int PULSE = ENC_SHOOT_BOTTOM_COUNT;
    ENC_SHOOT_BOTTOM_COUNT = 0;

    RPMF = float((PULSE * 3000.0) / 600.0);

    Error = rpm - RPMF;
    P = kp * Error;
    cumError += Error * TS;
    I = ki * cumError;
    rateError = (Error - lError) / TS;
    D = kd * rateError;
    PID = P + I + D;
    lError = Error;

    unsigned long elapsed = millis() - startTime;
    int targetPWM = map(rpm, 0, 7000, 0, 235);
    int rampPWM = map(elapsed, 0, 2000, 0, targetPWM);  // Feedforward yang sesuai

    if (rampPWM > targetPWM) rampPWM = targetPWM;

    if (rampPWM > 255) rampPWM = 255;


    // Gabungkan PWM ramp + PID

    // PWM_SHOOT_BOTTOM += PID;
    // PWM_SHOOT_BOTTOM += PID;
    if (rpm - RPMF <= 5) {
      PWM_SHOOT_BOTTOM += PID;
    } else {
      PWM_SHOOT_BOTTOM = rampPWM + factor * PID;
    }

    // Batas PWM
    if (PWM_SHOOT_BOTTOM > 255) PWM_SHOOT_BOTTOM = 255;
    else if (PWM_SHOOT_BOTTOM < 0) PWM_SHOOT_BOTTOM = 0;

    // Output PWM
    analogWrite(M_Shooter2_PIN, PWM_SHOOT_BOTTOM);

    // SerialUSB.print(RPMF);
    // SerialUSB.print("\t");
    // SerialUSB.print(rampPWM);
    // SerialUSB.print("\t");
    // SerialUSB.print(PID);
    // SerialUSB.print("\t");
    // SerialUSB.print(PWM_SHOOT_BOTTOM);
    // SerialUSB.println("\t");
  }
}


int Loss = 0;
int filtered_deRPM = 0, filtered_beRPM = 0, filtered_kaRPM = 0, filtered_kiRPM = 0;
unsigned long lTime8 = 0;
unsigned long lTime9 = 0;

// Deteksi apakah sedang dalam mode perlambatan (target = 0 atau perubahan arah)
bool isBrakingOrDirectionChange(int current, int target) {
  return (target == 0) || (current > 0 && target < 0) || (current < 0 && target > 0);
}

int rampRPM(int current, int target) {
  int step;
  int delta = abs(target - current);

  // Dead zone  (PWM di bawah 1 dianggap 0)
  if (abs(current) < 1 && current != 0) {
    current = 0;
    return current;
  }

  /*soft brake*/
  if (target == 0) {
    int abs_current = abs(current);

    if (abs_current <= 1) {
      return 0;
    }

    float decay_factor = 0.3;  // 30% reduction per step (sedikit lebih agresif)
    step = max(1, (int)(abs_current * decay_factor));

    step = max(step, 1);

    if (current > 0) {
      current = max(0, current - step);
    } else if (current < 0) {
      current = min(0, current + step);
    }

    if (abs(current) < 5) current = 0;

    return current;
  }

  /*soft change direction*/
  if ((current > 0 && target < 0) || (current < 0 && target > 0)) {
    int total_distance = abs(current) + abs(target);
    step = max(min(abs(current) / 4, abs(target) / 3), 5);
    const int MAX_STEP = 25;
    step = min(step, MAX_STEP);
    if (total_distance < 30) {
      step = max(2, step / 2);
    }
    if (current > target) {
      current -= step;
      if (current < target) current = target;
    } else {
      current += step;
      if (current > target) current = target;
    }
    return current;
  }

  /*softstart untuk meringankan kerja pid*/
  if (delta > 150) step = 15;
  else if (delta > 100) step = 10;
  else if (delta > 50) step = 5;
  else if (delta > 20) step = 2;
  else step = 1;

  if (current < target) {
    current += step;
    if (current > target) current = target;
  } else if (current > target) {
    current -= step;
    if (current < target) current = target;
  }

  return target;
}







void vSlave() {
  const int bufSize = 100;
  static char buf[bufSize];
  int deRPM, beRPM, kaRPM, kiRPM;
  uint8_t upState, downState, shotPos, shot, arm;
  int loss, rawBost, autoModeFlag;
  float bost;


  while (true) {
    if (slaveSerial.available()) {
      /*parsing data*/
      size_t len = slaveSerial.readBytesUntil('\n', buf, bufSize - 1);
      buf[len] = '\0';

      if (buf[0] == 'A') {  // Pastikan data diawali 'A'
        char* token = strtok(buf + 1, ",");
        if (token != NULL) kaRPM = atoi(token);
        token = strtok(NULL, ",");
        if (token != NULL) kiRPM = atoi(token);
        token = strtok(NULL, ",");
        if (token != NULL) beRPM = atoi(token);
        token = strtok(NULL, ",");
        if (token != NULL) deRPM = atoi(token);
        token = strtok(NULL, ",");
        if (token != NULL) upState = atoi(token);
        token = strtok(NULL, ",");
        if (token != NULL) downState = atoi(token);
        token = strtok(NULL, ",");
        if (token != NULL) shotPos = atoi(token);
        token = strtok(NULL, ",");
        if (token != NULL) shot = atoi(token);
        token = strtok(NULL, ",");
        if (token != NULL) arm = atoi(token);
        token = strtok(NULL, ",");
        if (token != NULL) loss = atoi(token);
        token = strtok(NULL, ",");
        if (token != NULL) M_Shooter1_PWM = atoi(token);
        token = strtok(NULL, ",");
        if (token != NULL) M_Shooter2_PWM = atoi(token);
        token = strtok(NULL, ",");
        if (token != NULL) rawBost = atoi(token);
        token = strtok(NULL, ",");
        if (token != NULL) autoModeFlag = atoi(token);

        // Batasi nilai RPM maksimal di 650 tetapi sisakan space untuk PID
        deRPM = constrain(deRPM, -550, 550);
        beRPM = constrain(beRPM, -550, 550);
        kaRPM = constrain(kaRPM, -550, 550);
        kiRPM = constrain(kiRPM, -550, 550);

        // Ramp filtering
        filtered_deRPM = rampRPM(filtered_deRPM, deRPM);
        filtered_beRPM = rampRPM(filtered_beRPM, beRPM);
        filtered_kaRPM = rampRPM(filtered_kaRPM, kaRPM);
        filtered_kiRPM = rampRPM(filtered_kiRPM, kiRPM);

        //rem tanpa softbrake
        if (loss) {
          filtered_deRPM = deRPM;
          filtered_beRPM = beRPM;
          filtered_kaRPM = kaRPM;
          filtered_kiRPM = kiRPM;
        }

        //nilai kp bost dari master
        bost = (float)rawBost / 10000;

        //aplikasi PID masing masing Motor
        RPMdepan(-filtered_deRPM, bost, 0.0001, 0.01);
        RPMbelakang(-filtered_beRPM, bost, 0.0001, 0.01);
        RPMkanan(-filtered_kaRPM, bost, 0.0001, 0.01);
        RPMkiri(-filtered_kiRPM, bost, 0.0001, 0.01);

        //pneumatic atas pendorong drible
        digitalWrite(sol1Up, upState);
        digitalWrite(sol1Down, !upState);

        if (shot) status_Shoot = true;
        if ((abs(RPME - M_Shooter1_PWM) <= 30 && abs(RPMF - M_Shooter2_PWM) <= 30) || M_Shooter1_PWM >= 8000) {
          status_Shoot1 = true;
        }

        if (!manualMode) {
          if (!autoMode) {
            digitalWrite(sol_Sudut_Up_PIN, shotPos);  //sudutPelontar
            digitalWrite(sol_Sudut_Down_PIN, !shotPos);
            digitalWrite(sol_Arm_Up, arm);  //arm masuk keluar
            digitalWrite(sol_Arm_Down, !arm);
            digitalWrite(sol2Up, downState);  //capit
            digitalWrite(sol2Down, !downState);
            lTime9 = millis();
          } else {
            unsigned long elapsedAuto = millis() - lTime9;
            if (elapsedAuto < 200) {
              digitalWrite(sol_Sudut_Down_PIN, HIGH);
              digitalWrite(sol_Sudut_Up_PIN, LOW);
            } else if (elapsedAuto < 700) {
              digitalWrite(sol_Arm_Up, LOW);  //arm masuk keluar
              digitalWrite(sol_Arm_Down, HIGH);
              RPMShootTop(M_Shooter1_PWM, 0.001, 0.0000, 0.000, 1);
              RPMShootBottom(M_Shooter2_PWM, 0.001, 0.0000, 0.000, 1);
            } else if (elapsedAuto < 1400) {
              digitalWrite(sol2Up, HIGH);  //capit
              digitalWrite(sol2Down, LOW);
              RPMShootTop(M_Shooter1_PWM, 0.001, 0.0000, 0.000, 1);
              RPMShootBottom(M_Shooter2_PWM, 0.001, 0.0000, 0.000, 1);
            } else if (elapsedAuto < 2300) {
              digitalWrite(sol2Up, LOW);  //capit
              digitalWrite(sol2Down, HIGH);

              digitalWrite(sol_Sudut_Down_PIN, LOW);
              digitalWrite(sol_Sudut_Up_PIN, HIGH);

              RPMShootTop(M_Shooter1_PWM, 0.003, 0.0000, 0.000, 1);
              RPMShootBottom(M_Shooter2_PWM, 0.003, 0.0000, 0.000, 1);
            } else if (elapsedAuto < 3100) {
              if (M_Shooter1_PWM >= 8000) {
                digitalWrite(sol_Sudut_Down_PIN, HIGH);
                digitalWrite(sol_Sudut_Up_PIN, LOW);
              }
              RPMShootTop(M_Shooter1_PWM, 0.004, 0.0000, 0.000, 1);
              RPMShootBottom(M_Shooter2_PWM, 0.004, 0.0000, 0.000, 1);
            } else if (elapsedAuto < 3600) {
              if (M_Shooter1_PWM >= 8000) {
                digitalWrite(sol_Sudut_Down_PIN, HIGH);
                digitalWrite(sol_Sudut_Up_PIN, LOW);
              } else {
                digitalWrite(sol_Sudut_Down_PIN, LOW);
                digitalWrite(sol_Sudut_Up_PIN, HIGH);
              }
              RPMShootTop(M_Shooter1_PWM, 0.004, 0.0000, 0.000, 1);
              RPMShootBottom(M_Shooter2_PWM, 0.004, 0.0000, 0.000, 1);
              lTime5 = millis();
            } else {
              if (M_Shooter1_PWM >= 8000) {
                digitalWrite(sol_Sudut_Down_PIN, HIGH);
                digitalWrite(sol_Sudut_Up_PIN, LOW);
              }
              if (status_Shoot && status_Shoot1) {
                unsigned long elapsed = millis() - lTime5;
                if (elapsed < 800) {
                  RPMShootTop(M_Shooter1_PWM, 0.004, 0.0000, 0.0000, 1);
                  RPMShootBottom(M_Shooter2_PWM, 0.004, 0.0000, 0.0000, 1);
                  digitalWrite(sol_Shooter_Up_PIN, HIGH);
                  digitalWrite(sol_Shooter_Down_PIN, LOW);
                } else {
                  RPMShootTop(0, 0.002, 0.0001, 0.001, 1);
                  RPMShootBottom(0, 0.002, 0.0001, 0.001, 1);
                  digitalWrite(sol_Shooter_Up_PIN, LOW);
                  digitalWrite(sol_Shooter_Down_PIN, HIGH);
                  status_Shoot = false;
                  status_Shoot1 = false;
                  autoMode = false;
                  lTime5 = millis();
                }
              } else {
                RPMShootTop(M_Shooter1_PWM, 0.004, 0.0000, 0.000, 1);
                RPMShootBottom(M_Shooter2_PWM, 0.004, 0.0000, 0.000, 1);
                // lTime9 = millis();
                lTime5 = millis();
              }
            }
          }
          lTime11 = millis();
        } else if (manualMode) {
          //manual mode masih bermasalah, kurang stabil, bisa disempurnakan lagi kedepannya
          unsigned long elapsedAuto = millis() - lTime11;
          digitalWrite(sol_Sudut_Up_PIN, shotPos);  //sudutPelontar
          digitalWrite(sol_Sudut_Down_PIN, !shotPos);
          digitalWrite(sol_Arm_Up, arm);  //arm masuk keluar
          digitalWrite(sol_Arm_Down, !arm);
          digitalWrite(sol2Up, downState);  //capit
          digitalWrite(sol2Down, !downState);
          if (status_Shoot) {
            if (elapsedAuto < 200) {
            } else if (elapsedAuto < 700) {
              RPMShootTop(M_Shooter1_PWM, 0.001, 0.0000, 0.000, 1);
              RPMShootBottom(M_Shooter2_PWM, 0.001, 0.0000, 0.000, 1);
            } else if (elapsedAuto < 1400) {
              RPMShootTop(M_Shooter1_PWM, 0.001, 0.0000, 0.000, 1);
              RPMShootBottom(M_Shooter2_PWM, 0.001, 0.0000, 0.000, 1);
            } else if (elapsedAuto < 2300) {
              RPMShootTop(M_Shooter1_PWM, 0.003, 0.0000, 0.000, 1);
              RPMShootBottom(M_Shooter2_PWM, 0.003, 0.0000, 0.000, 1);
            } else if (elapsedAuto < 3100) {
              RPMShootTop(M_Shooter1_PWM, 0.004, 0.0000, 0.000, 1);
              RPMShootBottom(M_Shooter2_PWM, 0.004, 0.0000, 0.000, 1);
            } else if (elapsedAuto < 3600) {
              RPMShootTop(M_Shooter1_PWM, 0.004, 0.0000, 0.000, 1);
              RPMShootBottom(M_Shooter2_PWM, 0.004, 0.0000, 0.000, 1);
              lTime5 = millis();
            } else {
              if (status_Shoot1) {
                unsigned long elapsed = millis() - lTime5;
                if (elapsed < 800) {
                  RPMShootTop(M_Shooter1_PWM, 0.006, 0.0000, 0.0000, 1);
                  RPMShootBottom(M_Shooter2_PWM, 0.006, 0.0000, 0.0000, 1);
                  digitalWrite(sol_Shooter_Up_PIN, HIGH);
                  digitalWrite(sol_Shooter_Down_PIN, LOW);
                } else {
                  RPMShootTop(0, 0.002, 0.0001, 0.001, 1);
                  RPMShootBottom(0, 0.002, 0.0001, 0.001, 1);
                  digitalWrite(sol_Shooter_Up_PIN, LOW);
                  digitalWrite(sol_Shooter_Down_PIN, HIGH);
                  status_Shoot = false;
                  status_Shoot1 = false;
                  autoMode = false;
                  lTime5 = millis();
                }
              } else {
                RPMShootTop(M_Shooter1_PWM, 0.004, 0.0000, 0.000, 1);
                RPMShootBottom(M_Shooter2_PWM, 0.004, 0.0000, 0.000, 1);
                // lTime9 = millis();
                lTime5 = millis();
              }
            }
          }
        }

        if (lastDownState == 1 && downState == 0 && arm && !autoModeFlag) {
          autoMode = true;
          manualMode = false;
        } else if (autoModeFlag) {
          manualMode = true;
          autoMode = false;
        }
        lastDownState = downState;
      }
    }
  }
}





void setup() {
  Serial.begin(57600);
  SerialUSB.begin(57600);
  slaveSerial.begin(115200);
  // espSerial.begin(115200);

  /* Encoder */
  pinMode(ENC_DE_CHA, INPUT);
  pinMode(ENC_BE_CHA, INPUT);
  pinMode(ENC_KA_CHA, INPUT);
  pinMode(ENC_KI_CHA, INPUT);
  pinMode(ENC_DE_CHB, INPUT);
  pinMode(ENC_BE_CHB, INPUT);
  pinMode(ENC_KA_CHB, INPUT);
  pinMode(ENC_KI_CHB, INPUT);

  pinMode(ENC_X_CHA, INPUT_PULLUP);
  pinMode(ENC_X_CHB, INPUT_PULLUP);
  pinMode(ENC_Y_CHA, INPUT_PULLUP);
  pinMode(ENC_Y_CHB, INPUT_PULLUP);

  pinMode(ENC_SHOOT_TOP_CHA, INPUT_PULLUP);
  pinMode(ENC_SHOOT_TOP_CHB, INPUT_PULLUP);
  pinMode(ENC_SHOOT_BOTTOM_CHA, INPUT_PULLUP);
  pinMode(ENC_SHOOT_BOTTOM_CHB, INPUT_PULLUP);

  /* Motor */
  pinMode(DE_RPWM_PIN, OUTPUT);
  pinMode(DE_LPWM_PIN, OUTPUT);
  pinMode(BE_RPWM_PIN, OUTPUT);
  pinMode(BE_LPWM_PIN, OUTPUT);
  pinMode(KA_RPWM_PIN, OUTPUT);
  pinMode(KA_LPWM_PIN, OUTPUT);
  pinMode(KI_RPWM_PIN, OUTPUT);
  pinMode(KI_LPWM_PIN, OUTPUT);

  /*solenoid*/
  pinMode(sol_Sudut_Up_PIN, OUTPUT);
  pinMode(sol_Sudut_Down_PIN, OUTPUT);
  pinMode(sol2Up, OUTPUT);
  pinMode(sol2Down, OUTPUT);
  pinMode(sol_Shooter_Up_PIN, OUTPUT);
  pinMode(sol_Shooter_Down_PIN, OUTPUT);
  pinMode(sol1Up, OUTPUT);
  pinMode(sol1Down, OUTPUT);
  pinMode(sol_Arm_Up, OUTPUT);
  pinMode(sol_Arm_Down, OUTPUT);

  /*shooter*/
  pinMode(M_Shooter1_PIN, OUTPUT);
  pinMode(M_Shooter2_PIN, OUTPUT);

  /*interrupt*/
  attachInterrupt(digitalPinToInterrupt(ENC_DE_CHA), ENC_DE_FUNCT, RISING);
  attachInterrupt(digitalPinToInterrupt(ENC_BE_CHA), ENC_BE_FUNCT, RISING);
  attachInterrupt(digitalPinToInterrupt(ENC_KA_CHA), ENC_KA_FUNCT, RISING);
  attachInterrupt(digitalPinToInterrupt(ENC_KI_CHA), ENC_KI_FUNCT, RISING);
  attachInterrupt(digitalPinToInterrupt(ENC_X_CHA), ENC_X_FUNCT, RISING);
  attachInterrupt(digitalPinToInterrupt(ENC_Y_CHA), ENC_Y_FUNCT, RISING);
  attachInterrupt(digitalPinToInterrupt(ENC_SHOOT_TOP_CHA), ENC_SHOOT_TOP_FUNCT, FALLING);
  attachInterrupt(ENC_SHOOT_BOTTOM_CHA, ENC_SHOOT_BOTTOM_FUNCT, FALLING);

  /*ini void loop nya untuk jalankan sesuai data yang diterima dari master*/
  vSlave(); 
  
  // trialPWM(100);
  // trialRPS();
  // trialEnc();
  // trialEnc();
  // trialPID();
  // analogWrite(DE_RPWM_PIN, 100);
  // digitalWrite(DE_LPWM_PIN, LOW);
  //  RPMdepan(200, 0.2, 0.000, 0.0);
}


void loop() {
  // readENC();
  // tesRPM();
  // readRPM(255);
  // checkSerialInput();
  // Serial.println(aaa);
  // trialPID();
  // coba();
}


#define CW 1
#define CCW 2
#define Stop 3

/* AT COMMAND */
#define AT_RESET "RESET"
#define AT_RPS "RPS"

/*COM*/
#define slaveSerial Serial2
#define REVOLUTIONA 94.0   //pg45 50KE7
#define REVOLUTIONB 134.0  //pg45 19.2K
/*
ternyata ada kesalahan disini, yang dipakai pada robot itu yang 19.2K
harusnya disesuaikan untuk PID yang lebih stabil, 
tapi sekarang its okey lah, belum di tuning untuk revolution yang sesuai....
*/

//shield lama
// #define ENC_DE_CHA 23
// #define ENC_DE_CHB 25
// #define ENC_BE_CHA 27
// #define ENC_BE_CHB 29
// #define ENC_KI_CHA 31
// #define ENC_KI_CHB 33
// #define ENC_KA_CHA 35
// #define ENC_KA_CHB 37
//shield v2
#define ENC_DE_CHA 27
#define ENC_DE_CHB 29
#define ENC_BE_CHA 31
#define ENC_BE_CHB 33
#define ENC_KI_CHA 35
#define ENC_KI_CHB 37
#define ENC_KA_CHA 41
#define ENC_KA_CHB 39

#define DE_RPWM_PIN 8
#define DE_LPWM_PIN 9
#define BE_RPWM_PIN 4
#define BE_LPWM_PIN 5
#define KI_RPWM_PIN 6
#define KI_LPWM_PIN 7
#define KA_RPWM_PIN 2
#define KA_LPWM_PIN 10 
//pin 3 jangan digunakan, short internal, panas.........

#define lSBawah_PIN 55
#define lSAtas_PIN 56

#define motorUp 13
#define motorDown 12

#define WHEEL_RADIUS 0.075  // Radius roda
const float linearVelocity = 2.0;

//deklarasi invers kinematic
float 
  w1, 
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
  ENC_KI_COUNT = 0;
volatile long int
  LENC_DE_COUNT = 0,
  LENC_BE_COUNT = 0,
  LENC_KA_COUNT = 0,
  LENC_KI_COUNT = 0;
float
  PWM_DE = 0,
  PWM_BE = 0,
  PWM_KA = 0,
  PWM_KI = 0;
unsigned long
  ltime,
  ltime1,
  ltime2,
  ltime3,
  lTime4,
  Previousmillis;

int lastPulses = 0;

volatile bool limitAtasAktif = false;
volatile bool limitBawahAktif = false;



// Global variables for soft stop
unsigned long previousMillis = 0;
const long interval = 80;  // 1 second interval for soft stop
int targetDeRPM = 0;
int targetBeRPM = 0;
int targetKaRPM = 0;
int targetKiRPM = 0;
int currentDeRPM = 0;
int currentBeRPM = 0;
int currentKaRPM = 0;
int currentKiRPM = 0;

//softstop, perlu pengembangan, untuk sekarang tidak dipakai pakainya rampRPM
void applySoftStop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    // Gradually reduce each RPM value towards zero
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
//hanya coba coba 
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


float aaa = 500, bbb = 0.1, ccc = 0.0, ddd = 0.0;  // Default values



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

void readENC() {               //resolusi
  Serial.print(ENC_DE_COUNT);  //94
  Serial.print("  ");
  Serial.print(ENC_BE_COUNT);  //134
  Serial.print("  ");
  Serial.print(ENC_KA_COUNT);  //134
  Serial.print("  ");
  Serial.print(ENC_KI_COUNT);  //94
  Serial.println("  ");
}

void tesRPM() {
  // analogWrite(KI_RPWM_PIN, 100);
  if (millis() - lTime4 >= 1000) {
    noInterrupts();
    int pulses = ENC_KI_COUNT;
    ENC_KI_COUNT = 0;
    interrupts();

    float rpmtes = (pulses / 94) * 60;
    float selisih = lastPulses - pulses;
    Serial.print(rpmtes);
    Serial.print("  ");
    Serial.println(selisih);

    // lastPulses = pulses;
    lTime4 = millis();
  }
  //depan 272
}

void checkSerialInput() {
  if (Serial.available() > 0) {
    char input[30];  
    int len = Serial.readBytesUntil('\n', input, sizeof(input) - 1);
    input[len] = '\0';  

    if (input[0] == '#') {  // Pastikan input pengguna diawali dengan #
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



void setup() {
  Serial.begin(115200);
  slaveSerial.begin(115200);

  /* Encoder */
  pinMode(ENC_DE_CHA, INPUT);
  pinMode(ENC_BE_CHA, INPUT);
  pinMode(ENC_KA_CHA, INPUT);
  pinMode(ENC_KI_CHA, INPUT);
  pinMode(ENC_DE_CHB, INPUT);
  pinMode(ENC_BE_CHB, INPUT);
  pinMode(ENC_KA_CHB, INPUT);
  pinMode(ENC_KI_CHB, INPUT);

  /* Motor */
  pinMode(DE_RPWM_PIN, OUTPUT);
  pinMode(DE_LPWM_PIN, OUTPUT);
  pinMode(BE_RPWM_PIN, OUTPUT);
  pinMode(BE_LPWM_PIN, OUTPUT);
  pinMode(KA_RPWM_PIN, OUTPUT);
  pinMode(KA_LPWM_PIN, OUTPUT);
  pinMode(KI_RPWM_PIN, OUTPUT);
  pinMode(KI_LPWM_PIN, OUTPUT);

  /*Limit Switch*/
  pinMode(lSAtas_PIN, INPUT_PULLUP);
  pinMode(lSBawah_PIN, INPUT_PULLUP);

  /* Motor naik turun*/
  pinMode(motorUp, OUTPUT);
  pinMode(motorDown, OUTPUT);

  /*interrupt*/
  attachInterrupt(digitalPinToInterrupt(ENC_DE_CHA), ENC_DE_FUNCT, RISING);
  attachInterrupt(digitalPinToInterrupt(ENC_BE_CHA), ENC_BE_FUNCT, RISING);
  attachInterrupt(digitalPinToInterrupt(ENC_KA_CHA), ENC_KA_FUNCT, RISING);
  attachInterrupt(digitalPinToInterrupt(ENC_KI_CHA), ENC_KI_FUNCT, RISING);
  // attachInterrupt(digitalPinToInterrupt(lSAtas_PIN), lSAtas_FUNCT, FALLING);
  // attachInterrupt(digitalPinToInterrupt(lSBawah_PIN), lSBawah_FUNCT, FALLING);


  /*gyro*/
  // Wire.begin();
  // byte status = mpu.begin();
  // if (status != 0) {
  //   Serial.println("Failed to connect to MPU6050.");
  // }
  // Serial.println(F("Calculating offsets, do not move MPU6050"));
  // mpu.calcOffsets();
  // mpu.update();
  // targetAngle = mpu.getAngleZ();
  // lTime4 = millis();

  // trialPWM(100);
  // trialRPS();
  
  


  vSlave();
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

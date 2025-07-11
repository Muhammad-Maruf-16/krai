// #include <PS2X_lib.h>
// #include <SoftwareSerial.h>
// #include <EEPROM.h>
// #include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Bluepad32.h>
#include <Preferences.h>

// Encoder pins
#define ENC_R_A 35
#define ENC_R_B 34
#define ENC_Y_A 33
#define ENC_Y_B 32
#define ENC_L_A 25
#define ENC_L_B 26

//simpan nilai encocder
volatile long tickR = 0, tickL = 0, tickY = 0;

// #define PPR 600
// #define WHEEL_CIRCUM 0.05 * 3.1416
// #define TICKS_PER_METER (PPR * 2 / WHEEL_CIRCUM)

//uart esp32 bebas mapping
#define RX2_PIN 19  //
#define TX2_PIN 16  //
#define RX1_PIN 17  //
#define TX1_PIN 18  //

LiquidCrystal_I2C lcd(0x27, 20, 4);  // Alamat 0x27, LCD 20x4

float yaw = 0.0;
float continuousYaw = 0.0;
float lastYaw = 0.0;
int rotationCount = 0;

ControllerPtr myController = nullptr;
uint8_t CALIBRATION_STEP = 1;
uint8_t CALIBRATION_JUMP = 50;
uint8_t DEADZONE_THRESHOLD = 5;
uint16_t Solenoid1 = 200;
uint16_t Solenoid2 = 300;
uint16_t delayTime = 0;
const int DEFAULT_UNINITIALIZED = -1;  // Changed from 255 to -1 to be more reliable
const int MAX_PWM = 600;
int stickLX = 128, stickLY = 128, stickRX = 128, stickRY = 128;
int DPAD_SPEED = 200;
int ROTATION_SPEED = 200;
int stickX, stickY;
int deRPM, beRPM, kaRPM, kiRPM;
int deRPM1, beRPM1, kaRPM1, kiRPM1;
int slowRote = 100;
int deOffset = 0;
int beOffset = 0;
int kaOffset = 0;
int kiOffset = 0;
int M1 = 0;
int M2 = 0;
int M3 = 0;
int M4 = 0;
int M1a = 0;
int M2a = 0;
int M3a = 0;
int M4a = 0;
int loss = 0;
// int speed = 200;
int shooter = 0;
int prevRPM = 100;
int stickLX_raw = 128;
int stickLY_raw = 128;
int stickRX_raw = 128;
int stickRY_raw = 128;
int drible_arm = 0;
int M_Shoot_Top = 2000;
int M_Shoot_Bottom = 2000;
int M_Shoot_Top1 = 2000;
int M_Shoot_Bottom1 = 2000;
int M_Shoot_Top2 = 2000;
int M_Shoot_Bottom2 = 2000;
int M_Shoot_Top3 = 2000;
int M_Shoot_Bottom3 = 2000;
int M_Shoot_Top4 = 2000;
int M_Shoot_Bottom4 = 2000;
int M_Shoot_Top5 = 8000;
int M_Shoot_Bottom5 = 8000;
int rawBost = 5;
int prev_analogs[4];
bool prev_buttons[16];
bool analogMode = true;
bool sol1 = 0;
bool sol2 = 1;
bool settingMode = false;
bool lastL3State = false;
bool lastR3State = false;
bool Up = false;
bool Down = false;
bool triangle = false;
bool cross = false;
bool circle = false;
bool square_ = false;
bool up = false;
bool down = false;
bool left = false;
bool right = false;
bool L1 = false;
bool R1 = false;
bool L2 = false;
bool R2 = false;
bool L3 = false;
bool R3 = false;
bool START = false;
bool SELECT = false;
bool blinkState = false;
bool alreadyPressedL1 = false;
bool alreadyPressedR1 = false;
bool alreadyPressedL2 = false;
bool alreadyPressedR2 = false;
bool alreadyPressedL3 = false;
bool alreadyPressedR3 = false;
bool alreadyPressedUP = false;
bool alreadyPressedDOWN = false;
bool alreadyPressedLEFT = false;
bool alreadyPressedRIGHT = false;
bool alreadyPressedSTART = false;
bool alreadyPressedTRIANGLE = false;
bool alreadyPressedSELECT = false;
bool alreadyPressedCROSS = false;
bool alreadyPressedCIRCLE = false;
bool autoModeFlag = false;
bool shooterPos = 0;
bool maxMode = false;
bool status_Shoot = false;
unsigned long lTime3 = 0;
unsigned long lTime1 = 0;
unsigned long lastSendTime = 0;

const int SEND_INTERVAL = 20;  // Kirim tiap 50 ms
Preferences prefs;

#define UPDATE_RATE_MS 20

//interrupt encoder esp32 2x decoding, 600ppr outputnya 1200ppr
void IRAM_ATTR encR_ISR() {
  bool A = digitalRead(ENC_R_A);
  bool B = digitalRead(ENC_R_B);
  tickR += (A == B) ? -1 : 1;
}

void IRAM_ATTR encL_ISR() {
  bool A = digitalRead(ENC_L_A);
  bool B = digitalRead(ENC_L_B);
  tickL += (A == B) ? -1 : 1;
}

void IRAM_ATTR encY_ISR() {
  bool A = digitalRead(ENC_Y_A);
  bool B = digitalRead(ENC_Y_B);
  tickY += (A == B) ? -1 : 1;
}


//hanya izinkan stick tertentu yang dapat terhubung,.. alamat mac bisa dilihat pakai library lengkap bluepad32 (stick ps3 clone)
bool isAllowedController(ControllerPtr ctl) {
  ControllerProperties p = ctl->getProperties();
  uint8_t allowedMac[] = { 0x98, 0xb6, 0xe1, 0x8b, 0xfa, 0x04 };
  return memcmp(p.btaddr, allowedMac, 6) == 0;
}


//ketika stick konek, jika bukan alamat mac yang terdaftar maka akan disconnect,...
void onConnectedController(ControllerPtr ctl) {
  if (!isAllowedController(ctl)) {
    ctl->disconnect();
    return;
  }

  if (myController == nullptr) {
    myController = ctl;
    ControllerProperties properties = ctl->getProperties();
  }
}

//ketika disconnect kosongkan data controller, agar siap terhubung
void onDisconnectedController(ControllerPtr ctl) {
  if (ctl == myController) myController = nullptr;
}

//ambil data terima dari stick ps3,.. juga beri nilai default ketika stick terputus,.
void processControllers() {
  if (myController && myController->isConnected()) {
    processGamepad(myController);
  } else {
    up = down = left = right =
      triangle = cross = circle = square_ =
        L1 = R1 = L2 = R2 = L3 = R3 = START = SELECT = 0;
    stickLX = stickLY = stickRX = stickRY = 128;
  }
}

//parsing data terima masukkan ke variabel agar lebih mudah
void processGamepad(ControllerPtr ctl) {
  uint8_t dpad = ctl->dpad();
  uint16_t btn = ctl->buttons();
  uint8_t miscc = ctl->miscButtons();

  up = (dpad == 0x01) || (dpad == 0x05) || (dpad == 0x09);
  down = (dpad == 0x02) || (dpad == 0x06) || (dpad == 0x0A);
  right = (dpad == 0x04) || (dpad == 0x06) || (dpad == 0x05);
  left = (dpad == 0x08) || (dpad == 0x09) || (dpad == 0x0A);

  START = (miscc == 0x04) || (miscc == 0x06);
  SELECT = (miscc == 0x02) || (miscc == 0x06);

  cross = btn & 0x0001;
  circle = btn & 0x0002;
  square_ = btn & 0x0004;
  triangle = btn & 0x0008;

  L1 = btn & 0x0010;
  R1 = btn & 0x0020;
  L2 = ctl->brake() > 50;  // L2 jadi true kalau ditekan lebih dari 50
  R2 = ctl->throttle() > 50;
  L3 = btn & 0x0100;
  R3 = btn & 0x0200;


  stickLX_raw = ctl->axisX();  // -512 to 511
  stickLY_raw = ctl->axisY();
  stickRX_raw = ctl->axisRX();
  stickRY_raw = ctl->axisRY();

  stickLX = (stickLX_raw + 512) >> 2;
  stickLY = (stickLY_raw + 512) >> 2;
  stickRX = (stickRX_raw + 512) >> 2;
  stickRY = (stickRY_raw + 512) >> 2;
}

//setup awal lcd
void setupLCD() {
  lcd.init();
  lcd.backlight();
  lcd.print("SPEED : ");
  lcd.print(DPAD_SPEED);
}

//setup encoder, encoder open kolektor butuh pullup, pullup esp32 terlalu lemah, dipakaikan pullup eksternal...
void setupEncoder() {
  pinMode(ENC_R_A, INPUT);
  pinMode(ENC_R_B, INPUT);
  pinMode(ENC_L_A, INPUT);
  pinMode(ENC_L_B, INPUT);
  pinMode(ENC_Y_A, INPUT);
  pinMode(ENC_Y_B, INPUT);
  attachInterrupt(digitalPinToInterrupt(ENC_R_A), encR_ISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENC_L_A), encL_ISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENC_Y_A), encY_ISR, CHANGE);
}

//setup gyro gy25
void setupGyro() {
  // Serial1.write(0xA5);
  // Serial1.write(0x54);
  // delay(1000);
  // Serial1.write(0xA5);
  // Serial1.write(0x55);
  // delay(1000);
  Serial1.write(0xA5);
  Serial1.write(0x53);
}

//read data gy25 diambil yaw saja
void readGY25() {
  if (Serial1.available()) {
    String data = Serial1.readStringUntil('\n');
    data.trim();

    // Format: #YPR=+000.17,+000.08,-000.25,
    if (data.startsWith("#YPR=")) {
      int firstComma = data.indexOf(',');
      if (firstComma > 5) {                             // 5 = panjang "#YPR="
        String yawStr = data.substring(5, firstComma);  // Skip "#YPR="
        float newYaw = yawStr.toFloat();

        //agar data output continuous tidak terbatas
        float diff = newYaw - lastYaw;
        if (diff > 180) {
          rotationCount--;
        } else if (diff < -180) {
          rotationCount++;
        }
        continuousYaw = rotationCount * 360.0 + newYaw;
        lastYaw = newYaw;
        yaw = newYaw;  // Raw yaw untuk referensi
      }
    }
  }
}

//tampilkan ke serial monitor
void debugEncoder() {
  Serial.print(" Ticks R: ");
  Serial.print(tickR);
  Serial.print(" | L: ");
  Serial.print(tickL);
  Serial.print(" | Y: ");
  Serial.print(tickY);
  Serial.print(" | Raw Yaw: ");
  Serial.print(yaw, 1);
  Serial.print("° | Continuous: ");
  Serial.print(continuousYaw, 1);
  Serial.print("° ");
}


//set speed
void setSpeed() {
  if (L2 && !alreadyPressedL2) {
    alreadyPressedL2 = true;
    DPAD_SPEED += 50;
    if (DPAD_SPEED > 400) DPAD_SPEED = 50;

    //kalibrasi belok agar mengikuti speed, perlu dikembangkan lebih lanjut,....
    M1 = ((long)M1 * DPAD_SPEED) / prevRPM;
    M2 = ((long)M2 * DPAD_SPEED) / prevRPM;
    M3 = ((long)M3 * DPAD_SPEED) / prevRPM;
    M4 = ((long)M4 * DPAD_SPEED) / prevRPM;
    // Serial.print(M1);
    // Serial.print("\t");
    // Serial.print(DPAD_SPEED);
    // Serial.print("\t");
    // Serial.print(prevRPM);
    // Serial.print("\t");
    prevRPM = DPAD_SPEED;
    //update tampilan lcd, by event jangan continuous...
    lcd.setCursor(6, 0);
    lcd.print(DPAD_SPEED);
    lcd.print("   ");
  } else if (!L2) {
    alreadyPressedL2 = false;
  }
}

void controlMode() {
  if (START) {
    autoModeFlag = true;
  } else if (SELECT) {
    autoModeFlag = false;
  }
}

void angleShoot() {
  if (R2 && !alreadyPressedR2) {
    alreadyPressedR2 = true;
    shooterPos = !shooterPos;
  } else if (!R2) {
    alreadyPressedR2 = false;
  }
}

void shooterFunct() {
  shooter = cross;
}

void armFunct() {
  if (triangle & !alreadyPressedTRIANGLE) {
    drible_arm = !drible_arm;
    alreadyPressedTRIANGLE = true;
  } else if (!triangle) {
    alreadyPressedTRIANGLE = false;
  }
}

void setSpeedFunct() {
  if (triangle) {
    M_Shoot_Top = M_Shoot_Top1;
    M_Shoot_Bottom = M_Shoot_Bottom1;
  } else if (square_) {
    M_Shoot_Top = M_Shoot_Top2;
    M_Shoot_Bottom = M_Shoot_Bottom2;
  } else if (circle) {
    M_Shoot_Top = M_Shoot_Top3;
    M_Shoot_Bottom = M_Shoot_Bottom3;
  } else if (cross) {
    M_Shoot_Top = M_Shoot_Top4;
    M_Shoot_Bottom = M_Shoot_Bottom4;
  } else if (R1) {
    M_Shoot_Top = M_Shoot_Top5;
    M_Shoot_Bottom = M_Shoot_Bottom5;
  } else {
    setSpeed();
  }
}

void processLogic() {
  if (!settingMode) {
    checkSettingMode();       //cek apakah masuk setting mode atau tidak
    calculateDpadMovement();  //kalkulasi dpad, dapat ditambahkan invers kinematics omni 4 wheel
    controlMode();            //manual Mode masih banyak bug di slave, silakan dikembangkan
    if (!L1) {
      driblee();       //drible
      angleShoot();    //sudut shoot
      shooterFunct();  //shooter
      armFunct();      //lengan drible
    } else {
      setSpeedFunct();  //preset rpm shooter & set rpm
    }
    sendToSlave();  //kirim ke slave
  } else {
    handleSettingMode();  //setting mode
  }
}


/*invers kinematics omni 4 wheel perlu pengembangan*/
#define MAX_RPM 600
#define L 20.0

void setOmniRPM(float Vx, float Vy, float Wz) {
  float M1a = Vy - Vx - Wz * L;
  float M2a = Vy + Vx + Wz * L;
  float M3a = Vy + Vx - Wz * L;
  float M4a = Vy - Vx + Wz * L;

  float maxVal = max(abs(M1a), max(abs(M2a), max(abs(M3a), abs(M4a))));
  if (maxVal > 1.0) {
    M1a /= maxVal;
    M2a /= maxVal;
    M3a /= maxVal;
    M4a /= maxVal;
  }

  M1a *= DPAD_SPEED;
  M2a *= DPAD_SPEED;
  M3a *= DPAD_SPEED;
  M4a *= DPAD_SPEED;
}



/*task bluetooth*/
void TaskBluetoothSend(void *pvParameters) {
  (void)pvParameters;
  const TickType_t delayTime = pdMS_TO_TICKS(SEND_INTERVAL);

  while (true) {
    // bool dataUpdated = BP32.update();
    // if (dataUpdated) processControllers();

    BP32.update();          //update stick ps3
    processControllers();   //proses kontroller
    processLogic();         //proses logic
    vTaskDelay(delayTime);  //delay internal, 20ms wajib
  }
}


/*untuk sensor encoder dan gyro, proses koordinat, perlu pengembangan*/
void TaskSensorRead(void *pvParameters) {
  (void)pvParameters;
  const TickType_t delayTime = pdMS_TO_TICKS(20);

  while (true) {
    // readGY25();  // Baca yaw dari GY-25
    // if (!settingMode) {
    // }
    // updateOdometry();
    // debugEncoder();  // Print tick encoder
    // debugGyroCorrection();
    vTaskDelay(delayTime);
  }
}

void readPrefferences() {
  prefs.begin("robot_1", true);  // true = read-only
  deOffset = prefs.getInt("deOffset", 0.0);
  beOffset = prefs.getInt("beOffset", 0.0);
  kaOffset = prefs.getInt("kaOffset", 0.0);
  kiOffset = prefs.getInt("kiOffset", 0.0);
  Solenoid1 = prefs.getInt("Solenoid1", 0);
  Solenoid2 = prefs.getInt("Solenoid2", 0);
  delayTime = prefs.getInt("delayTime", 0);
  M1 = prefs.getInt("M1", 0);
  M2 = prefs.getInt("M2", 0);
  M3 = prefs.getInt("M3", 0);
  M4 = prefs.getInt("M4", 0);
  slowRote = prefs.getInt("slowRote", 0);
  M_Shoot_Top1 = prefs.getInt("M_Shoot_Top1", 0);
  M_Shoot_Bottom1 = prefs.getInt("M_Shoot_Bottom1", 0);
  M_Shoot_Top2 = prefs.getInt("M_Shoot_Top2", 0);
  M_Shoot_Bottom2 = prefs.getInt("M_Shoot_Bottom2", 0);
  M_Shoot_Top3 = prefs.getInt("M_Shoot_Top3", 0);
  M_Shoot_Bottom3 = prefs.getInt("M_Shoot_Bottom3", 0);
  M_Shoot_Top4 = prefs.getInt("M_Shoot_Top4", 0);
  M_Shoot_Bottom4 = prefs.getInt("M_Shoot_Bottom4", 0);
  prefs.end();
}

void setup() {
  Serial.begin(115200);                                 //tx nya ke nano
  Serial1.begin(115200, SERIAL_8N1, RX1_PIN, TX1_PIN);  //17 rx, 16 tx, 17 dari gyro, 18 ke gyro
  Serial2.begin(115200, SERIAL_8N1, RX2_PIN, TX2_PIN);  //19 rx, 18 tx, 19 dari nano, 16 ke due(serial2)

  // motorRPM_mutex = xSemaphoreCreateMutex();
  setupLCD();
  BP32.setup(&onConnectedController, &onDisconnectedController);
  readPrefferences();

  M_Shoot_Top = M_Shoot_Top1;
  M_Shoot_Bottom = M_Shoot_Bottom1;
  ROTATION_SPEED = prevRPM = DPAD_SPEED;

  setupEncoder();
  setupGyro();

  xTaskCreatePinnedToCore(
    TaskBluetoothSend,
    "BluetoothTask",
    4096,
    NULL,
    1,
    NULL,
    0  // Core 0
  );

  xTaskCreatePinnedToCore(
    TaskSensorRead,
    "SensorTask",
    4096,
    NULL,
    1,
    NULL,
    1  // Core 1
  );
}

void loop() {
  //dead loop
}
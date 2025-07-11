// #include <PS2X_lib.h>
// #include <SoftwareSerial.h>
// #include <EEPROM.h>
// #include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Bluepad32.h>
#include <Preferences.h>

#define RX2_PIN 19  //
#define TX2_PIN 18  //
#define RX1_PIN 17  // 
#define TX1_PIN 16  // 

LiquidCrystal_I2C lcd(0x27, 20, 4);  // Alamat 0x27, LCD 20x4


ControllerPtr myController = nullptr;
uint8_t CALIBRATION_STEP = 1;
uint8_t CALIBRATION_JUMP = 50;
uint8_t DEADZONE_THRESHOLD = 5;
const int DEFAULT_UNINITIALIZED = -1;  // Changed from 255 to -1 to be more reliable
const int MAX_PWM = 600;
int DPAD_SPEED = 200;
int ROTATION_SPEED = 200;
int stickX, stickY;
int deRPM, beRPM, kaRPM, kiRPM;
int slowRote = 150;
int slowRoteB = 220;
int deOffset = 0;
int beOffset = 0;
int kaOffset = 0;
int kiOffset = 0;
int sol1 = 0;
int sol2 = 0;
int M1 = 100;
int M2 = -100;
int M3 = -30;
int M4 = 200;
int M1B = 100;
int M2B = -100;
int M3B = -30;
int M4B = 200;
int loss = 0;
int bost = 2;
int bost1 = 2;
int bost2 = 10;
int prevRPM = 100;
int stickLX_raw = 128;
int stickLY_raw = 128;
int stickRX_raw = 128;
int stickRY_raw = 128;
int stickLX = 128, stickLY = 128, stickRX = 128, stickRY = 128;
// int speed = 200;
int prev_analogs[4];
bool prev_buttons[16];
bool analogMode = true;
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
bool alreadyPressedCross = false;
bool alreadyPressedSquare = false;
bool alreadyPressedL1 = false;
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
unsigned long lastUpdate = 0;
unsigned long lastUpdate1 = 0;
unsigned long lTime3 = 0;
unsigned long lTime1 = 0;
unsigned long lastSendTime = 0;

const int SEND_INTERVAL = 20;  // Kirim tiap 20 ms
Preferences prefs;

#define UPDATE_RATE_MS 20

bool isAllowedController(ControllerPtr ctl) {
  ControllerProperties p = ctl->getProperties();
  uint8_t allowedMac[] = { 0x98, 0xB6, 0xAC, 0xE2, 0x79, 0x84 };  //zain
  // uint8_t allowedMac[] = { 0x98, 0xb6, 0xe1, 0x8b, 0xfa, 0x04 };  //edwin
  return memcmp(p.btaddr, allowedMac, 6) == 0;
}


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

void onDisconnectedController(ControllerPtr ctl) {
  if (ctl == myController) myController = nullptr;
}

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
  L2 = ctl->brake() > 200;  // L2 jadi true kalau ditekan lebih dari 50
  R2 = ctl->throttle() > 200;
  L3 = btn & 0x0100;
  R3 = btn & 0x0200;

  // stickLX = ctl->axisX();
  // stickLY = ctl->axisY();
  // stickRX = ctl->axisRX();
  // stickRY = ctl->axisRY();

  stickLX_raw = ctl->axisX();  // -512 to 511
  stickLY_raw = ctl->axisY();
  stickRX_raw = ctl->axisRX();
  stickRY_raw = ctl->axisRY();

  stickLX = (stickLX_raw + 512) >> 2;
  stickLY = (stickLY_raw + 512) >> 2;
  stickRX = (stickRX_raw + 512) >> 2;
  stickRY = (stickRY_raw + 512) >> 2;
}

void setupLCD() {
  lcd.init();
  lcd.backlight();
  lcd.print("SPEED : ");
  lcd.print(DPAD_SPEED);
}


void setSpeed() {
  if (L1 && !alreadyPressedL1) {
    alreadyPressedL1 = true;
    DPAD_SPEED += 50;
    if (DPAD_SPEED > 400) DPAD_SPEED = 50;

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
    lcd.setCursor(6, 0);
    lcd.print(DPAD_SPEED);
    lcd.print("   ");
  } else if (!L1) {
    alreadyPressedL1 = false;
  }
}

void processLogic() {
  if (!settingMode) {
    if (cross && !alreadyPressedCross) {
      alreadyPressedCross = true;
      DPAD_SPEED += 50;
      if (DPAD_SPEED > 400) DPAD_SPEED = 400;

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
      // Serial.print(prevRPM);
      // Serial.print("\t");
      lcd.setCursor(6, 0);
      lcd.print(DPAD_SPEED);
      lcd.print("   ");
    } else if (!cross) {
      alreadyPressedCross = false;
    }
    if (square_ && !alreadyPressedSquare) {
      alreadyPressedSquare = true;
      DPAD_SPEED -= 50;
      if (DPAD_SPEED < 150) DPAD_SPEED = 150;

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
      // Serial.print(prevRPM);
      // Serial.print("\t");
      lcd.setCursor(6, 0);
      lcd.print(DPAD_SPEED);
      lcd.print("   ");
    } else if (!square_) {
      alreadyPressedSquare = false;
    }
    if (circle) {
      bost = bost2;
    } else if (triangle) {
      bost = bost1;
    }

    checkSettingMode();
    calculateDpadMovement();

    Up = R1;
    Down = R2;

    sendToSlave();
  } else {
    handleSettingMode();
  }
}

unsigned long lTime12 = 0;

void TaskBluetoothSend(void *pvParameters) {
  (void)pvParameters;
  const TickType_t delayTime = pdMS_TO_TICKS(SEND_INTERVAL);

  while (true) {
    if (millis() - lTime12 >= 40) {
      // Proses stick tiap 40ms agar buffer tidak penuh
      BP32.update();
      processControllers();  
      lTime12 = millis();
    }
    processLogic();
    vTaskDelay(delayTime);  // Delay antar kirim
  }
}




void setup() {
  Serial.begin(115200);                                 //tx nya ke nano
  Serial1.begin(115200, SERIAL_8N1, RX1_PIN, TX1_PIN);  //17 rx, 16 tx, 17 dari gyro, 16 ke due(serial2)
  Serial2.begin(115200, SERIAL_8N1, RX2_PIN, TX2_PIN);  //19 rx, 18 tx, 19 dari nano, 18 ke gyro

  // motorRPM_mutex = xSemaphoreCreateMutex();
  setupLCD();
  BP32.setup(&onConnectedController, &onDisconnectedController);

  prefs.begin("robot_2", true);  // true = read-only

  deOffset = prefs.getInt("deOffset", 0.0);
  beOffset = prefs.getInt("beOffset", 0.0);
  kaOffset = prefs.getInt("kaOffset", 0.0);
  kiOffset = prefs.getInt("kiOffset", 0.0);
  M1 = prefs.getInt("M1", 0);
  M2 = prefs.getInt("M2", 0);
  M3 = prefs.getInt("M3", 0);
  M4 = prefs.getInt("M4", 0);
  slowRote = prefs.getInt("slowRote", 0);
  M1B = prefs.getInt("M1B", 0);
  M2B = prefs.getInt("M2B", 0);
  M3B = prefs.getInt("M3B", 0);
  M4B = prefs.getInt("M4B", 0);
  slowRoteB = prefs.getInt("slowRoteB", 0);

  bost = bost1;

  prefs.end();

  ROTATION_SPEED = prevRPM = DPAD_SPEED;


  xTaskCreatePinnedToCore(
    TaskBluetoothSend,
    "BluetoothTask",
    4096,
    NULL,
    1,
    NULL,
    1  // Core 0
  );
}

void loop() {
}
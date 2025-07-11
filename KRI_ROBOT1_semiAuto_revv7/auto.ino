//dalam proses pengembangan

#define MAX_RPM 600
#define L 30.0  // jarak pusat ke roda (cm)
#define PPR 400
#define DIAMETER_CM 5.0
#define WHEEL_CIRC (3.1416 * DIAMETER_CM)
#define COUNTS_PER_REV (PPR * 2)  // quadrature
#define TICKS_PER_CM (COUNTS_PER_REV / WHEEL_CIRC)

#define DISTANCE_TARGET_CM 300.0
#define HEADING_CORRECTION_GAIN 0.05

enum AutoState { IDLE,
                 MOVE_FORWARD,
                 ROTATE_180,
                 DONE };
AutoState autoState = IDLE;

long encoder_start = 0;
float yaw_start = 0;
float yaw_target = 0;
bool startt = false;
int correctionYaw = 0;

void autoMode() {
  float yaw_now = continuousYaw;  // dari GY-25 continuous yaw
  long encoder_Y = tickY;         // dari odometry roda tracking Y
  float distance_cm = (encoder_Y - encoder_start) / TICKS_PER_CM;

  if (START && !alreadyPressedSTART) {
    startt = !startt;
    alreadyPressedSTART = true;
  } else if (!START) {
    alreadyPressedSTART = false;
  }

  if (startt) {
    switch (autoState) {
      case IDLE:
        encoder_start = encoder_Y;
        yaw_start = yaw_now;
        yaw_target = yaw_start + 180.0;
        autoState = MOVE_FORWARD;
        break;

      case MOVE_FORWARD:
        if (distance_cm < DISTANCE_TARGET_CM) {
          float yaw_error = yaw_now - yaw_start;
          float Wz_correction = -yaw_error * HEADING_CORRECTION_GAIN;

          setOmniRPM(0.0, 1.0, Wz_correction);
        } else {
          setOmniRPM(0, 0, 0);
          autoState = ROTATE_180;
        }
        break;

      case ROTATE_180:
        {
          float error = yaw_target - yaw_now;
          if (abs(error) > 2.0) {
            float dir = (error > 0) ? 1.0 : -1.0;
            setOmniRPM(0, 0, dir);
          } else {
            setOmniRPM(0, 0, 0);
            autoState = DONE;
          }
        }
        break;

      case DONE:
        startt = false;
        break;
    }
  } else {
    autoState = IDLE;
    setOmniRPM(0, 0, 0);
  }
}




// Variabel posisi global
float posX = 0;
float posY = 0;

// Simpan tick lama
long lastTickX = 0;
long lastTickY = 0;

void updateOdometry() {
  if(START){
    correctionYaw = continuousYaw;
  }
  long currTickX = tickR;
  long currTickY = tickY;

  long dTickX = currTickX - lastTickX;
  long dTickY = currTickY - lastTickY;

  lastTickX = currTickX;
  lastTickY = currTickY;

  // Delta posisi lokal
  float dx_local = dTickX / TICKS_PER_CM;
  float dy_local = dTickY / TICKS_PER_CM;

  // Ambil heading saat ini (radian)
  float theta = radians(continuousYaw-correctionYaw);

  // Transformasi ke global
  float dx_global = dx_local * cos(theta) - dy_local * sin(theta);
  float dy_global = dx_local * sin(theta) + dy_local * cos(theta);

  posX += dx_global;
  posY += dy_global;
 
  // Debug Serial
  Serial.print("PosX: ");
  Serial.print(posX, 2);
  Serial.print(" cm\tPosY: ");
  Serial.print(posY, 2);
  Serial.print(" cm\tYaw: ");
  Serial.println(continuousYaw-correctionYaw, 2);
}

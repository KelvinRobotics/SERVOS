#include <Wire.h>
#include <HCPCA9685.h>

HCPCA9685 HCPCA9685(0x40);

// Servo configuration
#define SHOULDER_CHANNEL 3   // Shoulder on channel 3
#define ELBOW_CHANNEL 8      // Elbow on channel 8
#define L1 30                // Shoulder-to-elbow length (cm)
#define L2 30                // Elbow-to-end effector length (cm)

// PWM limits (empirical values)
#define SHOULDER_MIN 100     // 0° geometric (shoulder down)
#define SHOULDER_MAX 180     // 90° geometric (shoulder up)
#define ELBOW_MIN 60         // 0° geometric (elbow down)
#define ELBOW_MAX 200        // 90° geometric (elbow up)

void setup() {
  Serial.begin(9600);
  HCPCA9685.Init(SERVO_MODE);
  HCPCA9685.Sleep(false);
}

// Inverse kinematics for 2-DOF
bool inverseKinematics(float x, float y, int &shoulderAngle, int &elbowAngle) {
  float d = sqrt(x*x + y*y);
  if (d > L1 + L2) {
    Serial.println("Error: Target out of reach");
    return false;
  }

  // Calculate θ2 (elbow, negative solution for mechanical limits)
  float theta2 = -acos((x*x + y*y - L1*L1 - L2*L2) / (2 * L1 * L2));

  // Calculate θ1 (shoulder)
  float theta1 = atan2(y, x) - atan2(L2 * sin(theta2), L1 + L2 * cos(theta2));

  // Convert to degrees and map to PWM
  theta1 = degrees(theta1);
  theta2 = degrees(theta2);

  shoulderAngle = map(theta1, 0, 90, SHOULDER_MIN, SHOULDER_MAX);
  elbowAngle = map(theta2, 0, 90, ELBOW_MIN, ELBOW_MAX);

  return true;
}

// Smooth servo movement
void moveServo(int channel, int targetAngle) {
  int currentAngle = HCPCA9685.GetServoPosition(channel);
  int step = (targetAngle > currentAngle) ? 1 : -1;
  for (int i = currentAngle; i != targetAngle; i += step) {
    HCPCA9685.Servo(channel, i);
    delay(20);
  }
}

void loop() {
  // --- Point 1: (60, 0) ---
  float x1 = 60.0, y1 = 0.0;
  int shoulder1, elbow1;
  if (inverseKinematics(x1, y1, shoulder1, elbow1)) {
    Serial.print("Point (60, 0): Shoulder = ");
    Serial.print(shoulder1);
    Serial.print(", Elbow = ");
    Serial.println(elbow1);
    moveServo(SHOULDER_CHANNEL, shoulder1);
    moveServo(ELBOW_CHANNEL, elbow1);
    delay(2000);
  }

  // --- Point 2: (21.2, 51.2) ---
  float x2 = 21.2, y2 = 51.2;
  int shoulder2, elbow2;
  if (inverseKinematics(x2, y2, shoulder2, elbow2)) {
    Serial.print("Point (21.2, 51.2): Shoulder = ");
    Serial.print(shoulder2);
    Serial.print(", Elbow = ");
    Serial.println(elbow2);
    moveServo(SHOULDER_CHANNEL, shoulder2);
    moveServo(ELBOW_CHANNEL, elbow2);
    delay(2000);
  }
}

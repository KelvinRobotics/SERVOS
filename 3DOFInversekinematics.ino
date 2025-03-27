#include <Wire.h>
#include <HCPCA9685.h> // PWM servo driver library

HCPCA9685 HCPCA9685(0x40); // I2C address of PCA9685

// Servo channels
#define SHOULDER_CH 3   // Shoulder (vertical movement)
#define ELBOW_CH 8      // Elbow (vertical movement)
#define BASE_CH 9       // Base (lateral rotation)

// Arm segment lengths (cm)
#define L1 30           // Shoulder to elbow
#define L2 30           // Elbow to end effector

// PWM limits (calibrated values)
#define SHOULDER_MIN 100  // 0° geometric (down)
#define SHOULDER_MAX 180  // 90° geometric (up)
#define ELBOW_MIN 60      // 0° geometric (down)
#define ELBOW_MAX 200     // 90° geometric (up)
#define BASE_MIN 130      // -45° geometric (left)
#define BASE_MAX 200      // +45° geometric (right)

void setup() {
  Serial.begin(9600);
  HCPCA9685.Init(SERVO_MODE);
  HCPCA9685.Sleep(false); // Wake up PCA9685
}

// 3D Inverse Kinematics Calculation
bool inverseKinematics(float x, float y, float z, int &shoulderAngle, int &elbowAngle, int &baseAngle) {
  // Step 1: Calculate base rotation (θ₃)
  float phi = atan2(z, x); // Lateral angle in radians
  baseAngle = map(degrees(phi), -45, 45, BASE_MIN, BASE_MAX);

  // Step 2: Calculate shoulder/elbow in RY plane
  float r = sqrt(x*x + z*z); // Projection on XZ plane
  float d = sqrt(r*r + y*y); // 2D distance (RY plane)

  // Check reachability
  if (d > L1 + L2) {
    Serial.println("Error: Target out of reach");
    return false;
  }

  // Elbow angle (negative solution for "elbow down")
  float theta2 = -acos((d*d - L1*L1 - L2*L2) / (2 * L1 * L2));

  // Shoulder angle
  float theta1 = atan2(y, r) - atan2(L2 * sin(theta2), L1 + L2 * cos(theta2));

  // Convert to PWM values
  shoulderAngle = map(degrees(theta1), 0, 90, SHOULDER_MIN, SHOULDER_MAX);
  elbowAngle = map(degrees(theta2), 0, 90, ELBOW_MIN, ELBOW_MAX);

  return true;
}

// Smooth servo movement
void moveServo(int channel, int targetAngle) {
  int currentAngle = HCPCA9685.GetServoPosition(channel);
  int step = (targetAngle > currentAngle) ? 1 : -1;
  for (int i = currentAngle; i != targetAngle; i += step) {
    HCPCA9685.Servo(channel, i);
    delay(20); // Adjust for smoother motion
  }
}

void loop() {
  // Example 1: (x=60, y=0, z=0) → Arm extended forward
  float x1 = 60, y1 = 0, z1 = 0;
  int shoulder1, elbow1, base1;
  if (inverseKinematics(x1, y1, z1, shoulder1, elbow1, base1)) {
    Serial.print("Angles: Shoulder="); Serial.print(shoulder1);
    Serial.print(", Elbow="); Serial.print(elbow1);
    Serial.print(", Base="); Serial.println(base1);
    moveServo(BASE_CH, base1);
    moveServo(SHOULDER_CH, shoulder1);
    moveServo(ELBOW_CH, elbow1);
    delay(2000);
  }

  // Example 2: (x=21.2, y=51.2, z=21.2) → Arm bent up-right
  float x2 = 21.2, y2 = 51.2, z2 = 21.2;
  int shoulder2, elbow2, base2;
  if (inverseKinematics(x2, y2, z2, shoulder2, elbow2, base2)) {
    Serial.print("Angles: Shoulder="); Serial.print(shoulder2);
    Serial.print(", Elbow="); Serial.print(elbow2);
    Serial.print(", Base="); Serial.println(base2);
    moveServo(BASE_CH, base2);
    moveServo(SHOULDER_CH, shoulder2);
    moveServo(ELBOW_CH, elbow2);
    delay(2000);
  }
}

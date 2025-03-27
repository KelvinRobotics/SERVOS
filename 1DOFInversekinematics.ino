#include <Wire.h>
#include <HCPCA9685.h> // Using the HCPCA9685 library

HCPCA9685 HCPCA9685(0x40); // I2C address of PCA9685

// Servo configuration
#define SERVO_CHANNEL 3     // Servo channel (e.g., 3)
#define SERVO_MIN 100       // Minimum angle (100° = downward)
#define SERVO_MAX 180       // Maximum angle (180° = upward)
#define ARM_LENGTH 60       // Arm length in cm (60 cm)

void setup() {
  Serial.begin(9600);
  HCPCA9685.Init(SERVO_MODE); // Initialize PCA9685 in servo mode
  HCPCA9685.Sleep(false);     // Disable sleep mode
}

// Function to calculate angle θ for given (x, y)
int calculateAngle(float x, float y) {
  float distance = sqrt(x * x + y * y);
  
  // Check if the point is reachable (60 cm ± tolerance)
  if (abs(distance - ARM_LENGTH) > 0.5) {
    Serial.println("Error: Point is out of the arm's reach!");
    return -1; // Return -1 if unreachable
  }

  // Calculate angle in radians and convert to degrees
  float theta = atan2(y, x) * 180.0 / M_PI;
  
  // Map the angle to servo range (100°-180°)
  theta = constrain(theta, 0, 180); // Limit to 0°-180°
  theta = map(theta, 0, 180, SERVO_MIN, SERVO_MAX);
  
  return (int)theta;
}

// Move servo smoothly to a target angle
void moveServo(int targetAngle) {
  int currentAngle = HCPCA9685.GetServoPosition(SERVO_CHANNEL);
  int step = (targetAngle > currentAngle) ? 1 : -1;

  for (int i = currentAngle; i != targetAngle; i += step) {
    HCPCA9685.Servo(SERVO_CHANNEL, i);
    delay(20); // Adjust delay for smoother motion
  }
}

void loop() {
  // --- Example 1: Move to (42.43, 42.43) - 45° ---
  float x1 = 42.43;
  float y1 = 42.43;
  
  int angle1 = calculateAngle(x1, y1);
  if (angle1 != -1) {
    Serial.print("Moving to (42.43, 42.43). Angle: ");
    Serial.println(angle1);
    moveServo(angle1);
    delay(1000); // Wait 1 second
  }

  // --- Example 2: Move to (0, 60) - 90° ---
  float x2 = 0;
  float y2 = 60;
  
  int angle2 = calculateAngle(x2, y2);
  if (angle2 != -1) {
    Serial.print("Moving to (0, 60). Angle: ");
    Serial.println(angle2);
    moveServo(angle2);
    delay(1000);
  }

  // --- Return to initial position (100°) ---
  Serial.println("Returning to 100° (downward)");
  moveServo(SERVO_MIN);
  delay(1000);
}

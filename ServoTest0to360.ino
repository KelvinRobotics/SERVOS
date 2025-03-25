#include "HCPCA9685.h"  // PCA9685 library
#define I2CAdd 0x40     // I2C address
HCPCA9685 HCPCA9685(I2CAdd);  // Controller object
int i;  // Loop counter variable

void setup() {
    HCPCA9685.Init(SERVO_MODE);  // Initialize servo mode
    HCPCA9685.Sleep(false);      // Disable sleep mode
}

void loop() {
    // Move servos from 0° to 360°
    for (i=0;i<360;i++){
        HCPCA9685.Servo(0, i);  // Servo on channel 0
        HCPCA9685.Servo(1, i);  // Servo on channel 1
        HCPCA9685.Servo(2, i);  // Servo on channel 2
        HCPCA9685.Servo(3, i);  // Servo on channel 3
        HCPCA9685.Servo(4, i);  // Servo on channel 4
        HCPCA9685.Servo(5, i);  // Servo on channel 5
        HCPCA9685.Servo(6, i);  // Servo on channel 6
        HCPCA9685.Servo(7, i);  // Servo on channel 7
        delay(40);  // Delay for smooth movement
    }
    
    // Move servos from 360° to 0°
    for (i=360;i>0;i--){
        HCPCA9685.Servo(0, i);  // Servo on channel 0
        HCPCA9685.Servo(1, i);  // Servo on channel 1
        HCPCA9685.Servo(2, i);  // Servo on channel 2
        HCPCA9685.Servo(3, i);  // Servo on channel 3
        HCPCA9685.Servo(4, i);  // Servo on channel 4
        HCPCA9685.Servo(5, i);  // Servo on channel 5
        HCPCA9685.Servo(6, i);  // Servo on channel 6
        HCPCA9685.Servo(7, i);  // Servo on channel 7
        delay(40);  // Delay for smooth movement
    }
}

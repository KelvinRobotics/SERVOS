#include <HCPCA9685.h>
HCPCA9685 HCPCA9685(0x40);  // Dirección I2C del PCA9685

// Variables para control de velocidad
int velocidad = 20;  // Ajusta este valor (mayor = más lento, menor = más rápido)
int pasos = 10;      // Número de pasos para suavizar el movimiento

void setup() {
  HCPCA9685.Init(SERVO_MODE);
  HCPCA9685.Sleep(false);
}

void moverServos(int anguloInicial, int anguloFinal) {
  for (int paso = 0; paso <= pasos; paso++) {
    int anguloActual = map(paso, 0, pasos, anguloInicial, anguloFinal);
    for (int servoPin = 11; servoPin <= 15; servoPin++) {
      HCPCA9685.Servo(servoPin, anguloActual);
    }
    delay(velocidad);  // Controla la velocidad aquí
  }
}

void loop() {
  moverServos(0, 180);    // Mueve de 0° a 180° con velocidad graduable
  delay(500);             // Pausa entre movimientos
  moverServos(180, 0);    // Mueve de 180° a 0° con la misma velocidad
  delay(500);
}

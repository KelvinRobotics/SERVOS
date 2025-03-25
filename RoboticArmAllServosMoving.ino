#include <HCPCA9685.h>
HCPCA9685 HCPCA9685(0x40);  // Controlador PCA9685

// Definición de pines y límites de cada servo
const int dedos[5] = {11, 12, 13, 14, 15};  // Pines de los dedos (0° a 180°)
const int codo = 8;                         // Pin del codo (60° a 200°)
const int hombroFlexion = 3;                // Pin flexión hombro (100° a 180°)
const int hombroLateral = 9;                // Pin movimiento lateral hombro (130° a 200°)

// Posiciones iniciales (ajustables)
const int posInicialDedos = 0;    // Dedos abiertos
const int posInicialCodo = 60;     // Codo abajo
const int posInicialHombroFlex = 100; // Hombro flexionado abajo
const int posInicialHombroLat = 130;  // Hombro lateral centrado

// Variables para velocidad
const int velocidad = 20;  // Retardo entre pasos (ms)
const int pasos = 30;      // Pasos para movimientos suaves

void setup() {
  HCPCA9685.Init(SERVO_MODE);
  HCPCA9685.Sleep(false);
  moverAPosicionInicial();  // Coloca todos los servos en posición inicial al iniciar
  delay(1000);
}

void moverAPosicionInicial() {
  // Dedos (mano abierta)
  for (int i = 0; i < 5; i++) {
    HCPCA9685.Servo(dedos[i], posInicialDedos);
  }
  // Codo (abajo)
  HCPCA9685.Servo(codo, posInicialCodo);
  // Hombro (abajo y centrado)
  HCPCA9685.Servo(hombroFlexion, posInicialHombroFlex);
  HCPCA9685.Servo(hombroLateral, posInicialHombroLat);
  delay(500);
}

void moverServoSuave(int pin, int anguloInicial, int anguloFinal, int delayPasos = velocidad) {
  for (int paso = 0; paso <= pasos; paso++) {
    int anguloActual = map(paso, 0, pasos, anguloInicial, anguloFinal);
    HCPCA9685.Servo(pin, anguloActual);
    delay(delayPasos);
  }
}

void loop() {
  // --- Secuencia individual ---
  // 1. Mover dedos (cerrar mano)
  for (int i = 0; i < 5; i++) {
    moverServoSuave(dedos[i], posInicialDedos, 180);
  }
  delay(500);

  // 2. Mover codo (subir)
  moverServoSuave(codo, posInicialCodo, 200);
  delay(500);

  // 3. Mover hombro (flexión hacia arriba)
  moverServoSuave(hombroFlexion, posInicialHombroFlex, 180);
  delay(500);

  // 4. Mover hombro (lateral hacia arriba)
  moverServoSuave(hombroLateral, posInicialHombroLat, 200);
  delay(1000);

  // --- Movimiento coordinado ---
  // Todos los servos vuelven a la posición inicial simultáneamente
  for (int paso = 0; paso <= pasos; paso++) {
    // Dedos
    for (int i = 0; i < 5; i++) {
      HCPCA9685.Servo(dedos[i], map(paso, 0, pasos, 180, posInicialDedos));
    }
    // Codo
    HCPCA9685.Servo(codo, map(paso, 0, pasos, 200, posInicialCodo));
    // Hombro
    HCPCA9685.Servo(hombroFlexion, map(paso, 0, pasos, 180, posInicialHombroFlex));
    HCPCA9685.Servo(hombroLateral, map(paso, 0, pasos, 200, posInicialHombroLat));
    delay(velocidad);
  }
  delay(2000);  // Pausa antes de repetir
}

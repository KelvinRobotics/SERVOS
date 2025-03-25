#include <Servo.h> //librería servo de arduino

Servo myservo; //se identifica el servo
int myservoNEW=0, myservoPrev=0, myservoSmooth=0; //variables para controlar el servo
int flag1=0, flag2=0; //banderas para salir de posibles ciclos infinitos

void setup() {
  myservo.attach(5); //Indicamos el pin donde se encuentra el servo
  myservo.writeMicroseconds(1450); //posición inicial del servo

  //moveremos el servo desde la posición 1450 a la 900
  myservoNEW=900; //Nueva posición deseada
  myservoPrev=1450; //Posición anterior del servo

  while(myservoPrev > myservoNEW) { //mientras la posición anterior sea mayor a la nueva
    myservoSmooth=(myservoNEW * 0.01)+(myservoPrev * 0.99); //operación matemática para suavizar
    myservo.writeMicroseconds(myservoSmooth);
    myservoPrev = myservoSmooth;
    delay(25);
  }

  //moveremos el servo desde la posición 900 a la 1450
  myservoNEW=1450; //nueva posición deseada

  while(myservoPrev < myservoNEW) { //mientras la posición nueva sea mayor a la anterior
    flag1=myservoPrev; //guardamos la posición anterior en la bandera 1
    myservoSmooth=(myservoNEW * 0.02)+(myservoPrev * 0.98); //operación matemática para suavizar
    myservo.writeMicroseconds(myservoSmooth);
    myservoPrev = myservoSmooth;
    flag2=myservoSmooth; //guardamos la posición que se envió al servo en la bandera 2
    if(flag1 == flag2){ //si son iguales bandera 1 con bandera 2 entonces
      myservoPrev = myservoPrev + 1; //al valor anterior del servo se le suma 1
      //para así romper cualquier posibilidad de un ciclo infinito
    }
    delay(25);
  }
}

void loop() {
}

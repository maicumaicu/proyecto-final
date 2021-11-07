#define CANT_BTN 3
#define ESPERA 0
#define CONFIRMACION 1
#define LIBERACION 2
#define TICKS_BTN  25

#define PIERNA_DERECHA 0
#define PIERNA_IZQUIERDA 1
#define BRAZO_DERECHA 2
#define BRAZO_IZQUIERDA 3
#define PECHO 4

#define RESULT 1
#define ON 2

struct button {
  public:
    int pin;
    int estado;
    int flag;
};
button btns[CANT_BTN];
#include <SoftwareSerial.h>
#include <TimerOne.h>
SoftwareSerial BTserial(10, 11); // RX | TX
int j, lecturaBtn, ticks, timer, ms;
int choice;
bool correct;
int estadoIotino = RESULT   ;
char c = ' ';

void setup() {
  // put your setup code here, to run once:
  Timer1.initialize(1000);
  Timer1.attachInterrupt(tick);
  Serial.begin(9600);
  Serial.println("Arduino with HC-06 is ready");

  // HC-06 default baud rate is 9600
  BTserial.begin(9600);
  Serial.println("BTserial started at 9600");
  for (int i = 2 , j = 0; j < CANT_BTN; j++, i++) {
    btns[j].pin = i;
    btns[j].estado = ESPERA;
    btns[j].flag = 0;
    pinMode(btns[j].pin, INPUT_PULLUP);
  }
}

void loop()
{
  for (j = 0; j < CANT_BTN; j++) {
    btnMachine(j);
  }
  IotinoMachine();
}

void IotinoMachine() {
  switch (estadoIotino) {
    case ON:
      for (int x = 0; x < CANT_BTN; x ++) {
        if (btns[x].flag == 1) {
          BTserial.write(x);
          estadoIotino = RESULT;
          Serial.print("hola");
        }
      }
      break;
    case RESULT:
      if (BTserial.available()) {
        c = BTserial.read();
        if (c == '1') {
          estadoIotino = ON;
          Serial.print("chau");
        }
      }
      break;
  }
}

void btnMachine(int index) {
  switch (btns[index].estado) {
    case ESPERA:
      btns[index].flag = 0;
      lecturaBtn = digitalRead(btns[index].pin);
      if (lecturaBtn == 0) {
        ticks = 0;
        btns[index].estado = CONFIRMACION;
      }
      break;

    case CONFIRMACION:
      lecturaBtn = digitalRead(btns[index].pin);
      if ( lecturaBtn == 0 && ticks >= TICKS_BTN) {
        btns[index].estado = LIBERACION;
      }

      if ( lecturaBtn == 1 && ticks < TICKS_BTN) {
        btns[index].estado = ESPERA;
      }
      break;

    case LIBERACION:
      lecturaBtn = digitalRead(btns[index].pin);
      if ( lecturaBtn == 1) {
        btns[index].flag = 1;
        btns[index].estado = ESPERA;
      }
      break;
  }
}

void tick() {
  ticks++;
  timer++;
  ms++;
}

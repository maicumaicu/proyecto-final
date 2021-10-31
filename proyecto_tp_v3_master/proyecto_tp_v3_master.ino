#define CANT_BTN 2
#define ESPERA 0
#define CONFIRMACION 1
#define LIBERACION 2
#define TICKS_BTN  25

#define PIERNA_DERECHA 0
#define PIERNA_IZQUIERDA 1
#define BRAZO_DERECHA 2
#define BRAZO_IZQUIERDA 3
#define PECHO 4

#define ON 0
#define RESULT 1
#define START 2

struct button {
  public:
    int pin;
    int estado;
    int flag;
};
button btns[CANT_BTN];
#include <SoftwareSerial.h>
#include <TimerOne.h>
SoftwareSerial BTSerial(10, 11); // RX | TX
int j, lecturaBtn, ticks, timer, part, ms;
int choice;
bool correct;
int estadoIotino = START;

void setup()
{
  Timer1.initialize(1000);
  Timer1.attachInterrupt(tick);
  Serial.begin(9600);
  Serial.println("Arduino with HC-06 is ready");

  // HC-06 default baud rate is 9600
  BTSerial.begin(9600);
  Serial.println("BTserial started at 9600");
  for (int i = 2 , j = 0; j < CANT_BTN; j++, i++) {
    btns[j].pin = i;
    btns[j].estado = ESPERA;
    btns[j].flag = 0;
    pinMode(btns[j].pin, INPUT_PULLUP);
  }
  part = randomPartSelecter();
}

void loop()
{
  for (j = 0; j < CANT_BTN; j++) {
    btnMachine(j);
  }
  IotinoMachine();
}

int randomPartSelecter() {
  int part = random(0, CANT_BTN);
  Serial.println(part);
  return part;
}

void IotinoMachine() {
  switch (estadoIotino) {

    case START:

      if (BTSerial.available()) {
        estadoIotino = ON;
      } else {
        BTSerial.write(part);
      }
      break;
    case ON:
      for (int x = 0; x < CANT_BTN; x ++) {
        if (btns[x].flag == 1) {
          estadoIotino = RESULT;
          choice = x;
          if (part == choice) {
            BTSerial.write('1');
            Serial.print(0);
          } else {
            BTSerial.write('0');
          }
        }
      }
      ms = 0;
      break;
    case RESULT:
      Serial.println(ms);
      if (ms >= 1000) {
        Serial.print(0);
        BTSerial.write('2');
      }
      if (BTSerial.available()) {
        char d = BTSerial.read();
        Serial.print(d);
        if (d == '1') {
          Serial.print(BTSerial.read());
          estadoIotino = START;
          part = randomPartSelecter();
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

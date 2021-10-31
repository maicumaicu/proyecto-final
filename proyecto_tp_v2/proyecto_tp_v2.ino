#include <TimerOne.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif


#define CANT_BTN 4
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

#define PIN_MATRIZ       12 // On Trinket or Gemma, suggest changing this to 1

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 64 // Popular NeoPixel ring size

Adafruit_NeoPixel pixels(NUMPIXELS, PIN_MATRIZ, NEO_GRB + NEO_KHZ800);

struct button {
  public:
    int pin;
    int estado;
    int flag;
};

struct part {
  public:
    int r;
    int g;
    int b;
};

part body[CANT_BTN];
button btns[CANT_BTN];
int j, lecturaBtn, ticks, timer, part, choice, ms;
bool correct;
int estadoIotino = PIERNA_DERECHA;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Timer1.initialize(1000);
  Timer1.attachInterrupt(tick);
  pixels.begin();
  pixels.clear();
  pixels.setBrightness(10);

  for (int i = 2 , j = 0; j < CANT_BTN; j++, i++) {
    btns[j].pin = i;
    btns[j].estado = ESPERA;
    btns[j].flag = 0;
    pinMode(btns[j].pin, INPUT_PULLUP);
    body[j].r = 255;
    body[j].g = 0;
    body[j].b = 0;
  }
  part = randomPartSelecter();
}

void loop() {
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

    case ON:
      prenderParte(body[part].r, body[part].g, body[part].b, part);
      for (int x = 0; x < CANT_BTN; x ++) {
        if (btns[x].flag == 1) {
          estadoIotino = RESULT;
          choice = x;
          pixels.clear();
          pixels.show();
          ms = 0;
        }
      }
      break;

    case RESULT:
      if (part == choice) {
        victoria();
      } else {
        derrota();
      }
      if (ms > 1000) {
        estadoIotino = ON;
        part = randomPartSelecter();
        pixels.clear();
        pixels.show();
      }
      break;
  }
}

void victoria() {
  for (int x = 0; x < NUMPIXELS; x ++) {
    if (x == 1) {
      pixels.setPixelColor(0, pixels.Color(0, 125, 0));
    } else if (x == 2) {
      pixels.setPixelColor(0, pixels.Color(0, 0, 0));
    } else if (x == 0) {
      pixels.setPixelColor(0, pixels.Color(0, 255, 0));
    }
    pixels.setPixelColor(x + 2, pixels.Color(0, 255, 0));
    pixels.setPixelColor(x + 1, pixels.Color(0, 125, 0));
    pixels.setPixelColor(x, pixels.Color(0, 0, 0));
    pixels.show();
  }
}

void derrota() {
  for (int x = 0; x < NUMPIXELS; x ++) {
    pixels.setPixelColor(x, pixels.Color(255, 0, 0));
    pixels.show();
  }
}

void prenderParte(int red, int green, int blue, int parte) {
  for (int x = parte; x < NUMPIXELS; x += 8) {
    pixels.setPixelColor(x, pixels.Color(red, green, blue));
    pixels.show();
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

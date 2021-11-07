#include <SoftwareSerial.h>
#include <Adafruit_NeoPixel.h>
#include <TimerOne.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#define ON         0
#define GUESS      1
#define RESULT     2

#define CANT_BTN 3
#define PIN_MATRIZ 12
#define NUMPIXELS  5

SoftwareSerial BTserial(10, 11); // RX | TX
Adafruit_NeoPixel pixels(NUMPIXELS, PIN_MATRIZ, NEO_GRB + NEO_KHZ800);

struct part {
  public:
    int r;
    int g;
    int b;
};

part body[CANT_BTN];
int choice, screenState, ms, part;
bool correct;
char d = ' ';
char c = ' ';

void setup() {
  // put your setup code here, to run once:
  Timer1.initialize(1000);
  Timer1.attachInterrupt(tick);
  // start the serial communication with the host computer
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  Serial.println("Arduino with HC-05 is ready");

  // start communication with the HC-05 using 9600
  BTserial.begin(9600);
  Serial.println("BTserial started at 9600");
  pixels.begin();
  pixels.clear();
  pixels.show();
  pixels.setBrightness(10);
  for (int j = 0; j < CANT_BTN; j++) {
    body[j].r = 255;
    body[j].g = 0;
    body[j].b = 0;
  }
  part =  randomPartSelecter();
}

void loop() {
  // put your main code here, to run repeatedly:
  screenMachine();
}

void screenMachine() {
  switch (screenState) {
    case ON:
      pixels.clear();
      pixels.show();
      prenderParte(body[part].r, body[part].g, body[part].b, part);
      pixels.show();
      BTserial.write('1');
      screenState = GUESS;
      Serial.println("hola");
      break;
    case GUESS:
      if (BTserial.available()) {
        c = BTserial.read();
        Serial.println((int)c);
        Serial.println(part);
        if (c == part) {
          correct = true;
        }
        screenState = RESULT;

        Serial.println(correct);
        ms = 0;
      }
      break;
    case RESULT:
      if (correct == true) {
        victoria();
      } else {
        derrota();
      }
      if (ms > 1000) {
        Serial.print("Bye");
        screenState = ON;
        correct = false;
        part =  randomPartSelecter();
      }

      break;
  }
}

int randomPartSelecter() {
  int part = random(0, CANT_BTN);
  Serial.println(part);
  return part;
}


void prenderParte(int red, int green, int blue, int parte) {
  pixels.setPixelColor(parte, pixels.Color(red, green, blue));
  pixels.show();
}

void victoria() {
  for (int x = 0; x < NUMPIXELS; x ++) {
    pixels.setPixelColor(x, pixels.Color(0, 255, 0));
    pixels.show();
  }
}

void derrota() {
  for (int x = 0; x < NUMPIXELS; x ++) {
    pixels.setPixelColor(x, pixels.Color(255, 0, 0));
    pixels.show();
  }
}

void tick() {
  ms++;
}

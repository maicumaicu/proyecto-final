
#include <SoftwareSerial.h>
#include <Adafruit_NeoPixel.h>
#include <TimerOne.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#define ON         0
#define GUESS      1
#define RESULT     2

#define CANT_BTN 2
#define PIN_MATRIZ 12
#define NUMPIXELS  64

SoftwareSerial BTserial(10, 11); // RX | TX
Adafruit_NeoPixel pixels(NUMPIXELS, PIN_MATRIZ, NEO_GRB + NEO_KHZ800);

struct part {
  public:
    int r;
    int g;
    int b;
};

part body[CANT_BTN];
int choice, screenState, ms;
char d = ' ';
char c = ' ';

void setup()
{
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
}

void loop()
{
  screenMachine();

}

void screenMachine() {
  switch (screenState) {
    case ON:
      pixels.clear();
      pixels.show();
      if (BTserial.available())
      {
        Serial.println(0);
        c = BTserial.read();
        for (int x = 0; x < CANT_BTN; x ++) {
          if (c == x) {
            prenderParte(body[c].r, body[c].g, body[c].b, c);
            screenState = GUESS;
            BTserial.write(1);
            ms = 0;
          }
        }
      }

      break;
    case GUESS:

      if (BTserial.available()) {
        c = BTserial.read();
        Serial.println(c);
      }
      if (c == '1') {
        Serial.println(3);
        victoria();
        screenState = RESULT;
      } else if (c == '0') {
        derrota();
        Serial.println(3);
        screenState = RESULT;
      }

      break;
    case RESULT:
      if (BTserial.available()) {
        d = BTserial.read();
        Serial.println(d);
        if (d == '2') {
          BTserial.write('1');
          screenState = ON;
          Serial.println(2);
        }
      } else {
        if (c == '1') {
          Serial.println(3);
          victoria();
        } else if (c == '0') {
          derrota();
          Serial.println(3);
        }
      }


      break;
  }
}

void prenderParte(int red, int green, int blue, int parte) {
  for (int x = parte; x < NUMPIXELS; x += 8) {
    pixels.setPixelColor(x, pixels.Color(red, green, blue));
    pixels.show();
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

void tick() {
  ms++;
}

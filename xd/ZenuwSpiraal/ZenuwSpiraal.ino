#include <TimerOne.h>
#include <Wire.h>
#include <MultiFuncShield.h>
/*Omdat low en high ongewisseld zijn hebben wij onze eigen low en high constanten
   geimplementeerd
*/
const int low = 1;
const int high = 0;


//leds
const int pog1 = 10;
const int pog2 = 11;
const int pog3 = 12;

//inputs
int startt = A1;
int draad = A2;
int eind = A3;

unsigned long t = 0;
unsigned long dt = 0;

static unsigned long tijd = 0;
unsigned long refTijd = 0 ;
float ftijd = 0;

static int poging = 1;
static float highscore = 100.00;
static float gameHighscore = 100.00;

void setup() {
  Timer1.initialize();
  MFS.initialize(&Timer1);

  pinMode(pog1, OUTPUT);
  pinMode(pog2, OUTPUT);
  pinMode(pog3, OUTPUT);

  pinMode(startt, INPUT);
  pinMode(draad, INPUT);
  pinMode(eind, INPUT);

  //pieper
  pinMode(3, OUTPUT);
}

void toonhighscore() {
  char str[] = "Highscore   ";
  MFS.blinkDisplay(DIGIT_ALL, OFF);

  for (int i = 0; i < 9; i++) {
    MFS.write(str[i], 0);
    MFS.write(str[i + 1], 1);
    MFS.write(str[i + 2], 2);
    MFS.write(str[i + 3], 3);
    delay(200);
  }
  MFS.blinkDisplay(DIGIT_ALL, ON);
  MFS.write(gameHighscore,2);
  delay(1500);
  MFS.blinkDisplay(DIGIT_ALL, OFF);
}

void toonpoging() {
  if (poging == 1) {

    digitalWrite(pog1, low);
    digitalWrite(pog2, low);
    digitalWrite(pog3, high);
  } else if (poging == 2) {
    digitalWrite(pog1, low);
    digitalWrite(pog2, high);
    digitalWrite(pog3, high);
  } else {
    digitalWrite(pog1, high);
    digitalWrite(pog2, high);
    digitalWrite(pog3, high);
  }
}


void loop() {

  //pieper

  if (poging != 4) {
    dt = millis();
    if (dt - t < 2000) {
      MFS.write("PRESS");
      MFS.blinkDisplay(DIGIT_ALL, ON);
    } else if (dt - t >= 2000 && dt - t < 4000) {
      MFS.write("PLAY");
      MFS.blinkDisplay(DIGIT_ALL, ON);
    } else {
      t = millis();
    }
    if (digitalRead(draad)==high){
      toonhighscore();
    }
    if (digitalRead(startt) == high) {
      MFS.blinkDisplay(DIGIT_ALL, OFF);
      ftijd = 0;


      for (int i = 3; i > 0; i--) {
        MFS.write(i);
        digitalWrite(3, high);
        delay(250);
        digitalWrite(3, low);
        delay(250);
      }
      MFS.write("GO", 2);
      digitalWrite(3, high);
      delay(500);
      digitalWrite(3, low);
      toonpoging();
      refTijd = millis();

      while (true) {

        MFS.write(ftijd, 2);
        if ( ftijd == 99.99) {

        } else {
          tijd = millis();
          ftijd = (tijd - refTijd) / 1000.00;

        }
        if (digitalRead(draad) == high ) {

          digitalWrite(3, high);
          delay(200);
          digitalWrite(3, low);
          poging++;
          break;
        }

        if (digitalRead(eind) == high) {

          if (ftijd < highscore) {
            highscore = ftijd;
          }
          MFS.blinkDisplay(DIGIT_ALL, ON);
          delay(2000);
          MFS.blinkDisplay(DIGIT_ALL, OFF);
          poging++;
          break;
        }

      }
    }
  } else {
    if (highscore < gameHighscore ) {
      gameHighscore = highscore;
    }

    digitalWrite(pog1, low);
    digitalWrite(pog2, low);
    digitalWrite(pog3, low);
    t = millis();
    dt = 0;
    bool gedrukt = true;

    while (gedrukt == true) {
      if (digitalRead(draad) == high || digitalRead(startt) == high || digitalRead(eind) == high) {
        gedrukt = false;
      }
      dt = millis();
      if (dt - t < 2000) {
        MFS.write("EIND");
        MFS.blinkDisplay(DIGIT_ALL, ON);
      } else if (dt - t >= 2000 && dt - t < 4000) {
        MFS.write(highscore, 2);
        MFS.blinkDisplay(DIGIT_ALL, OFF);
      } else {
        t = millis();
      }
    } poging = 1;
  }
}

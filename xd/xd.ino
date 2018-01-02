#include <TimerOne.h>
#include <Wire.h>
#include <MultiFuncShield.h>

static bool gstart = false;
static bool gdraad = false;
static bool geind = false;

const int LSTART = 10;
const int LDRAAD = 11;
const int LEIND = 12;

int startt = A1;
int draad = A2;
int eind = A3;

static unsigned long tijd = 0;
unsigned long refTime = 0 ;
float ftijd = 0;
void setup() {
  Timer1.initialize();
  MFS.initialize(&Timer1);

  pinMode(LSTART, OUTPUT);
  pinMode(LDRAAD, OUTPUT);
  pinMode(LEIND, OUTPUT);

  pinMode(startt, INPUT);
  pinMode(draad, INPUT);
  pinMode(eind, INPUT);

  pinMode(3, OUTPUT);

  digitalWrite(LSTART, HIGH);
  digitalWrite(LDRAAD, HIGH);
  digitalWrite(LEIND, HIGH);

}
void ledOnOF(bool geheugen, int led) {
  if (geheugen == true) {
    digitalWrite(led, LOW);
  } else {
    digitalWrite(led, HIGH);
  }
}
void startTimer(bool geheugen) {
  tijd = millis();

}
void resetTimer() {
  tijd = millis();
}
void loop() {
  if (digitalRead(startt) == LOW) {
    gstart = true;
    gdraad  = false;
    geind = false;
  }
  if (digitalRead(draad) == LOW || digitalRead(eind) == LOW) {
    gstart = false;
    if (digitalRead(draad) == LOW) {
      if (geind == true) {
        gdraad = false;
      } else {
        gdraad = true;
      }
    }
    if (digitalRead(eind) == LOW) {
      if (gdraad == true) {
        geind = false;
      } else {
        geind = true;
      }
    }
  }
  if (digitalRead(startt) == LOW) {
    tijd = millis();
    refTime = millis();
  }

  tijd = millis();
  ftijd = (tijd - refTime) / 1000.00;
  if (gdraad == true || geind == true) {
    //pause
  } else {
    MFS.write(ftijd, 2);
  }
  //pieper
  if (geind == false) {
    if (digitalRead(draad) == LOW) {
      digitalWrite(3, LOW);
    } else {
      digitalWrite(3, HIGH);
    }
  }

  ledOnOF(gstart, LSTART);
  ledOnOF(gdraad, LDRAAD);
  ledOnOF(geind, LEIND);



}

#include <TimerOne.h>
#include <Wire.h>
#include <MultiFuncShield.h>
/*Omdat low en high ongewisseld zijn hebben wij onze eigen low en high constanten
   geimplementeerd
*/
const int low = 1;
const int high = 0;

//geheugen
static bool gstart = false;
static bool gdraad = false;
static bool geind = false;

//leds
const int LSTART = 10;
const int LDRAAD = 11;
const int LEIND = 12;

//inputs
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

  //pieper
  pinMode(3, OUTPUT);
}
//Led gaat aan of uit afhankelijk van de waarde van de geheugen
void ledOnOF(bool geheugen, int led) {
  if (geheugen == true) {
    digitalWrite(led, high);
  } else {
    digitalWrite(led, low);
  }
}

void loop() {
  if (digitalRead(startt) == high) {
    gstart = true;
    gdraad  = false;
    geind = false;
  }
  if (digitalRead(draad) == high || digitalRead(eind) == high) {
    gstart = false;
    if (digitalRead(draad) == high) {
      if (geind == true) {
        gdraad = false;
      } else {
        gdraad = true;
      }
    }
    if (digitalRead(eind) == high) {
      if (gdraad == true) {
        geind = false;
      } else {
        geind = true;
      }
    }
  }
  if (digitalRead(startt) == high) {
    tijd = millis();
    refTime = millis();
    ftijd=0.00;
  }


  if (gdraad == true || geind == true || ftijd == 99.99) {
    //pause
  } else {
    tijd = millis();
    ftijd = (tijd - refTime) / 1000.00;
    MFS.write(ftijd, 2);
  }
  //pieper
  if (geind == false) {
    if (digitalRead(draad) == high) {
      digitalWrite(3, high);
    } else {
      digitalWrite(3, low);
    }
  }

  ledOnOF(gstart, LSTART);
  ledOnOF(gdraad, LDRAAD);
  ledOnOF(geind, LEIND);



}

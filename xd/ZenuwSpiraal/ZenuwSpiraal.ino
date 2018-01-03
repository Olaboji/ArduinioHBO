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
static bool ground = false;
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
unsigned long t = 0;
unsigned long dt = 0;

int rounds = 1;
static float highscore = 100.00;

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
  while (rounds < 4) {
   
    MFS.write(rounds);
    if (digitalRead(startt) == high) {
      gstart = true;
      gdraad  = false;
      geind = false;
    }
    if (gdraad == true || geind == true || ftijd == 99.99) {
      //pause
    } else {
      tijd = millis();
      ftijd = (tijd - refTime) / 1000.00;
      //MFS.write(ftijd, 2);
    }
    if (digitalRead(draad) == high || digitalRead(eind) == high) {
      gstart = false;
      ground = true;
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
          if (ftijd < highscore) {
            highscore = ftijd;
          }
        }
      }
    }


    //pieper
    if (geind == false) {
      if (digitalRead(draad) == high) {
        digitalWrite(3, high);
      } else {
        digitalWrite(3, low);
      }
    }
    if (digitalRead(startt) == high) {
      tijd = millis();
      refTime = millis();
      ftijd = 0.00;
    }

    if (ground == true && rounds == 3) {
      rounds = 0;
      break;
    } else if (ground == true && gstart == true) {
      rounds++;
      ground = false;
    }


    ledOnOF(gstart, LSTART);
    ledOnOF(gdraad, LDRAAD);
    ledOnOF(geind, LEIND);

    
  }
  MFS.write("Eind");
  delay(1000);
  MFS.write(highscore, 2);
  delay(1000);
}

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

//houdt tijd verschil bij 
unsigned long t = 0;
unsigned long dt = 0;

//houdt tijd bij highscore
static unsigned long tijd = 0;
unsigned long refTijd = 0 ;
float ftijd = 0;

static int poging = 1;

static float gameHighscore = 100.00;

void setup() {
  Timer1.initialize();
  MFS.initialize(&Timer1);
//poging ledjes
  pinMode(pog1, OUTPUT);
  pinMode(pog2, OUTPUT);
  pinMode(pog3, OUTPUT);

//buttons
  pinMode(startt, INPUT);
  pinMode(draad, INPUT);
  pinMode(eind, INPUT);

  //pieper
  pinMode(3, OUTPUT);
}
//toont de highscore 
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
//toont de poging met de ledjes 
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
   //highscore is undefined wanneer er geen highscore is 
   static float highscore = 100.00;
  //houdt bij welke poging 
  if (poging != 4) {
    dt = millis();
    //Wisselt tussen PRESS and PLAY elke 2000 ms
    if (dt - t < 2000) {
      MFS.write("PRESS");
      MFS.blinkDisplay(DIGIT_ALL, ON);
    } else if (dt - t >= 2000 && dt - t < 4000) {
      MFS.write("PLAY");
      MFS.blinkDisplay(DIGIT_ALL, ON);
    } else {
      t = millis();
    }
    //toont highscore 
    if (digitalRead(draad)==high){
      toonhighscore();
    }
    //begint poging 
    if (digitalRead(startt) == high) {
      MFS.blinkDisplay(DIGIT_ALL, OFF);
      ftijd = 0;

       //piep aftellen en countdown op led display 
      for (int i = 3; i > 0; i--) {
        MFS.write(i);
        digitalWrite(3, high);
        delay(250);
        digitalWrite(3, low);
        delay(250);
      }
      //GO!
      MFS.write("GO", 2);
      digitalWrite(3, high);
      delay(500);
      digitalWrite(3, low);
      toonpoging();
      //relatieve tijd = 0; 
      refTijd = millis();
      //oneindige loop tot break.
      while (true) {
        //stopt wanneer tijd 99.99
        MFS.write(ftijd, 2);
        if ( ftijd == 99.99) {

        } else {
          tijd = millis();
          ftijd = (tijd - refTijd) / 1000.00;

        }
        //breakt wanneer draad geraakt word 
        if (digitalRead(draad) == high ) {
          //biept voor 200 ms zodat je het kan horen
          digitalWrite(3, high);
          delay(200);
          digitalWrite(3, low);
          poging++;
          break;
        }
        //breakt wanneer eind geraakt wordt en kijkt als hjet een highscore is 
        if (digitalRead(eind) == high) {
          //slaat de tijd op als het de snelste tijd was 
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
    //slaat de snelste tijd op van alle games
    if (highscore < gameHighscore ) {
      gameHighscore = highscore;
    }
    //alle leds uit
    digitalWrite(pog1, low);
    digitalWrite(pog2, low);
    digitalWrite(pog3, low);
    t = millis();
    dt = 0;
    bool gedrukt = true;

    //wacht tot en met de gebruiker de 'gedrukt' knop drukt en reset de poging daarna
    while (gedrukt == true) {
      if ( digitalRead(eind) == high) {
        gedrukt = false;
      }
      //Wisselt tussen EIND en de highscore waarde elke 2000 ms
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
    }//reset poging 
    poging = 1;
  }
}

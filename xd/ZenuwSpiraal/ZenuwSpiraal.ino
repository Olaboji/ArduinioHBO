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
static bool gcountdown = false;
//leds
const int LSTART = 10;
const int LDRAAD = 11;
const int LEIND = 12;

//inputs
int startt = A1;
int draad = A2;
int eind = A3;

//button 
int S1;
static unsigned long tijd = 0;
unsigned long refTijd = 0 ;
unsigned long pieptijd = 0;
unsigned long refpieptijd = 0;
float ftijd = 0;

int ronde = 1;
static float highscore = 100.00;
static float gameHighscore = 100.00;
char poging[4][4] = {"----", "pog1", "pog2", "pog3"};
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
void toonhighscore(){
 if(highscore < gameHighscore ){
  gameHighscore = highscore;
 }
 MFS.blinkDisplay(DIGIT_ALL, ON); // display laten knipperen
  delay(500);
  MFS.blinkDisplay(DIGIT_ALL, OFF); // display knipperen laten stoppen
  MFS.write(gameHighscore,2);
}


void loop() {
  while (ronde < 4) {
    //pieper
  

    if (digitalRead(startt) == high) {
      gstart = true;
      gdraad  = false;
      geind = false;
      MFS.write(poging[ronde]);
    }else{
      MFS.write(ftijd, 2);
    }

    if (gdraad == true || geind == true || ftijd == 99.99) {
      
    } else {
      tijd = millis();
      ftijd = (tijd - refTijd) / 1000.00;
      
    }
  
    if (digitalRead(draad) == high || digitalRead(eind) == high) {
      gstart = false;
      ground = true;
      if (digitalRead(draad) == high && digitalRead(startt)==low) {
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
      tijd = millis();
      refTijd = millis();
      ftijd = 0.00;
    }
    pieptijd = millis();

    //pieper
    if (geind == false) {

      if (digitalRead(draad) == high   ) {
        digitalWrite(3, high);
        refpieptijd = millis();
      } else if (pieptijd - refpieptijd < 100) {
        digitalWrite(3, high);
      }
      else {
        digitalWrite(3, low);
      }
    }

    if (ground == true && ronde == 3) {
      ronde = 0;
      break;
    } else if (ground == true && gstart == true) {
      ronde++;
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
  ftijd = highscore;
}

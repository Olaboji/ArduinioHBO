/* In dit programma gaan we een nummerraadspel maken met een highscore functie en rondes
 *  A1 is teller 
 *  A2 is voor de highscore 
 *  A3 is om naar de ronde scherm te gaan, met A1 en A2 kan je de hoeveelheid rondes bepalen. Door a3 weer te drukken verlaat je de rondes
   
    Door Hubert Do (17078660) en Olasoji makinwa (17029139);


*/
#include <TimerOne.h>
#include <Wire.h>
#include <MultiFuncShield.h>
#include <math.h>
const int buttonPin = A1;
const int buttonPinHighScore = A2;

static int rounds = 1;
static int roundcounter = 1;
static float average = 0;

int buttonStateHighScore = 0;
int buttonStateRound = 0;
int teller = 0;
int buttonState = 0;
int lastState = LOW;
int randNumber;
static float highscore = 0;

char hexDigits[6] = {'A', 'B', 'C', 'D', 'E', 'F'};

unsigned long timeBegin = 0;
unsigned long timeDebounce = 0;
unsigned long timeStart = 0;
unsigned long timeEnd = 0;
unsigned long  timeFinish = 0;

// Deciamal to binary
void displayBinary(int decimalNumber) {
  for (int exponent = 3; exponent >= 0; exponent--) {

    if (pow(2, exponent) > decimalNumber) {
      digitalWrite(10 + exponent, HIGH);
    } else {
      digitalWrite(10 + exponent, LOW);
      decimalNumber = decimalNumber - pow(2, exponent);

    }
  }
}
void setup() {
  Serial.begin(9600);
  //Intialize multifuntion shield
  Timer1.initialize();
  MFS.initialize(&Timer1);
  // Intialize pins as output
  for (int pin = 10; pin < 14; pin++) {
    pinMode(pin, OUTPUT);
  }

  //A5 wordt als seed gebruikt omdat die niet wordt gebruikt voor ander onderdelen
  randomSeed(analogRead(A5));
  pinMode(buttonPin, INPUT);
  //De tijd  van de duur van de programma
  timeStart = millis();
}
void displayString(char str[], int duration = 500) {
  int len = strlen(str);
  char copystr[len + 3];
  strcpy (copystr, str);
  copystr[len] = ' ';
  copystr[len + 1] = ' ';
  copystr[len + 2] = ' ';

  for (int i = 0; i < len; i++) {
    MFS.write(copystr[i], 0);
    MFS.write(copystr[i + 1], 1);
    MFS.write(copystr[i + 2], 2);
    MFS.write(copystr[i + 3], 3);
    delay(duration);
  }
}

void loop() {

  randNumber = random(1, 16);
  displayBinary(randNumber);
  teller = 0;
  MFS.write(0);
  //Tijd om te kijken als er 2 seconden verstreken zijn.
  timeBegin = millis();
  //Oneindige loop tot en met 2 seconde verstreken zijn
  while (true) {
    //Elke iteratie wordt de tijd bijgehouden
    timeEnd = millis();
    buttonState = digitalRead(buttonPin);
    buttonStateHighScore = digitalRead(buttonPinHighScore);
    buttonStateRound = digitalRead(A3);
    //laat het highscore schermpje zien
    if (buttonStateHighScore == LOW) {
      char score[] = "Highscore";
      displayString(score, 200);
      if (highscore == 0) {
        MFS.write("----");
      } else {
        MFS.write(highscore, 1);
        delay(500);
      }
      delay(500);
      break;
    }
    //laat het nr. of rounds schermpje zien
    if (buttonStateRound == LOW) {
      char nrRounds[] = "Nr. of Rounds";
      displayString(nrRounds, 200);
      MFS.write(rounds);
      while (true) {
        //increment ronde
        if (digitalRead(A1) == LOW) {
          rounds++;
          MFS.write(rounds);
          delay(300);
        } else if (digitalRead(A2) == LOW) {
          if (rounds != 1) {
            //decrement ronde
            rounds--;
            MFS.write(rounds);
            delay(300);
          }
        } else if (digitalRead(A3) == LOW) {
          roundcounter = rounds;
          break;
        }
      }
      delay(300);
      return 0;
    }

    if (buttonState == LOW ) {
      //increment teller wanneer je op de button drukt
      teller++;
      //verschil in timeEnd en timeBegin  wordt nul, de gebruiker krijgt weer 2 seconde om te drukken
      timeBegin = millis();
      timeEnd = millis();

      if (teller == 16) {
        teller = 1;
      } else {
        //nothing
      }
      if (teller < 10) {
        MFS.write(teller);
      } else {
        MFS.write(hexDigits[teller - 10], 3);
      }
      //debounce
      while (buttonState != HIGH) {
        delay(20);
        buttonState = digitalRead(buttonPin);
        //houdt bij hoe lang debounce bezig is
        timeDebounce = millis();
        //debounce tijd wordt opgeteld bij timeEnd
        timeEnd = timeEnd + 20 + (timeDebounce - timeEnd);
        //als je de button ingedrukt houdt, houdt het nog steeds bij als
        //er 2 seconde verstreken zijn
        if (timeDebounce - timeBegin - 20 > 2000) {
          break;

        }
      }

    } else {
      //niks
    }

    if (timeEnd - timeBegin > 2000) {
      //wanneer de 2 seconde verstreken zijn geeft het aan hoeveel seconden het duurde om de juiste aantal
      //tellen te krijgen
      timeFinish = millis();
      //omdat er twee seconde vertraging is nadat de gebruiker de juiste waarde heeft gekozen wordt  er 2000 ms ervan gehaald
      timeFinish = timeFinish - 2000;
      float finished = timeFinish - timeStart;
      // ms naar s
      finished = finished / 1000;
      Serial.println("Buttonpress");
      Serial.println(finished);

      if (teller == randNumber) {
        float foo = finished;
        MFS.write("Goed");
        delay(500);
        // MFS.write(finished, 1);
        if (rounds == 1) {

          MFS.write(finished, 1);

        } else {
          //deeld door de aantal rondes 
          roundcounter--;
          average += finished;
          if (roundcounter == 0) {
            finished = average / rounds;
            MFS.write(finished, 1);
            roundcounter = rounds;
            average = 0;
            delay(500);
          }
        }
        if (highscore == 0) {
          highscore = finished;
        } else if (foo < highscore) {
          highscore = foo;
        }
     

      } else {
        MFS.write("Fout");
        delay(500);

      }
      break;
    }
  }

  delay(500);
  //begin tijd 
  timeStart = millis();

}

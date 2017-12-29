#include "star_wars.h"

#include <LiquidCrystal.h>

void setup() {
    auto sw = StarWars::GetInstance();

    sw.PrintTitle();

    pinMode(8, OUTPUT);
}

//void loop() {
  // Turn off the display:
  //lcd.noDisplay();
  //delay(500);
  //lcd.clear();
  // Turn on the display:
  //lcd.display();
  //delay(500);
//}





// void setup() {

// pinMode(9, OUTPUT);
// digitalWrite(9,LOW);

// }

void loop() {
    StarWars::PlayAudio();

    auto sw = StarWars::GetInstance();

    //sw.FadeTitle();
}

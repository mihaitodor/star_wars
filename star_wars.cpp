#include <Arduino.h>

#include "star_wars.h"

namespace {
    constexpr uint16_t blinkTitleDuration = 5000;
    constexpr uint16_t blinkStepDuration = 500;

    constexpr uint16_t crawlStepDuration = 1500;

    constexpr char* crawlStrings[]={
        "It is a period",
        "of civil war.",
        "Rebel",
        "spaceships,",
        "striking from a",
        "hidden base,",
        "have won their",
        "first victory",
        "against the evil",
        "Galactic Empire.",
        "",
        "During the",
        "battle, Rebel",
        "spies managed",
        "to steal secret",
        "plans to the",
        "Empire's",
        "ultimate weapon,",
        "the DEATH STAR,",
        "an armored space",
        "station with",
        "enough power to",
        "destroy an",
        "entire planet.",
        "",
        "Pursued by the",
        "Empire's",
        "sinister agents,",
        "Princess Leia",
        "races home",
        "aboard her",
        "starship,",
        "custodian of the",
        "stolen plans",
        "that can save",
        "her people and",
        "restore freedom",
        "to the galaxy...",
        "",
        ""
    };

    constexpr uint8_t crawlStringsCount = sizeof(crawlStrings) / sizeof(crawlStrings[0]);

    constexpr unsigned long delayBeforeEnding = 1000;
}

/*
 Set up a 16x2 LCD display. The LiquidCrystal library works with all
 LCD displays that are compatible with the Hitachi HD44780 driver.
 There are many of them out there, and you can usually tell them by
 the 16-pin interface.

 The circuit:
 * LCD Register Select RS pin (pin 4) to digital pin 13
 * LCD Enable E pin (pin 6) to digital pin 12
 * LCD D0 pin (pin 7) to digital pin 11
 * LCD D1 pin (pin 8) to digital pin 10
 * LCD D2 pin (pin 9) to digital pin 9
 * LCD D3 pin (pin 10) to digital pin 8
 * LCD D4 pin (pin 11) to digital pin 7
 * LCD D5 pin (pin 12) to digital pin 6
 * LCD D6 pin (pin 13) to digital pin 5
 * LCD D7 pin (pin 14) to digital pin 4
 * LCD R/W pin (pin 5) to ground
 * LCD backlight anode A pin (pin 15) to +5V through a 220 ohm resistor
 * LCD backlight cathode K pin (pin 16) to ground
 * LCD VSS pin (pin 0) to ground
 * LCD VDD pin (pin 1) to +5V
 * 10k potentiometer to LCD display contrast VO pin (pin 3)
*/
StarWars::StarWars() :
    startButtonPin_(2),
    //                 rs, en,  d0,  d1, d2, d3, d4, d5, d6, d7
    lcd_(LiquidCrystal(13, 12,  11,  10,  9,  8,  7,  6,  5,  4)),
    // Pin 3
    buzzerPin_(3) {

    // initialize the start button pin as an input:
    pinMode(startButtonPin_, INPUT);

    // Set up the LCD's number of columns and rows
    lcd_.begin(16, 2);
}

StarWars &StarWars::GetInstance() {
    static StarWars sw;

    return sw;
}

void StarWars::PlayPause() {
    if (digitalRead(startButtonPin_) == HIGH) {
        isRunning_ = !isRunning_;

        if (startMillis_ == 0) {
            startMillis_ = millis();
            blinkRunning_ = true;
        }

        // Guard against fat finger long presses
        delay(500);
    }
}

void StarWars::PrintTitle() {
    lcd_.clear();
    lcd_.setCursor(6, 0);
    lcd_.print("Star");
    lcd_.setCursor(6, 1);
    lcd_.print("Wars");
}

void StarWars::BlinkTitle() {
    if (!blinkRunning_) { return; }

    auto currentMillis = millis();

    if (currentMillis - startMillis_ > blinkTitleDuration) {
        if (!displayIsOn_) {
            lcd_.display();
        }

        blinkRunning_ = false;
        crawlRunning_ = true;

        return;
    }

    if (currentMillis > displayBlinkMillis_ + blinkStepDuration) {
        displayBlinkMillis_ = currentMillis;

        if (displayIsOn_) {
            lcd_.noDisplay();
        } else {
            lcd_.display();
        }
        displayIsOn_ = !displayIsOn_;
    }
}

void StarWars::ShowOpeningCrawl() {
    if (!crawlRunning_) { return; }

    if (currCrawlStep_ >= crawlStringsCount) {
        crawlRunning_ = false;
        showEnding_ = true;
        return;
    }

    auto currentMillis = millis();

    // Wait crawlStepDuration before starting the crawl
    if (currentMillis - displayBlinkMillis_ < crawlStepDuration) {
        return;
    }

    if (currentMillis > crawlMillis_ + crawlStepDuration) {
        crawlMillis_ = currentMillis;

        if (currCrawlStep_ == 0) {
            lcd_.clear();
            lcd_.setCursor(0, 1);
            lcd_.print(crawlStrings[0]);

            ++currCrawlStep_;
            return;
        }

        lcd_.clear();
        lcd_.print(crawlStrings[currCrawlStep_ - 1]);
        lcd_.setCursor(0, 1);
        lcd_.print(crawlStrings[currCrawlStep_]);
        ++currCrawlStep_;
    }
}

void StarWars::ShowEnding() {
    if (!showEnding_) { return; }

    auto currentMillis = millis();

    // Wait delayBeforeEnding before showing the ending
    if (currentMillis - crawlMillis_ < delayBeforeEnding) {
        return;
    }

    lcd_.clear();
    lcd_.print("May The Force be");
    lcd_.setCursor(4, 1);
    lcd_.print("with you!");

    showEnding_ = false;
}

void StarWars::AddNote(float freq, float duration, bool doubleDelay = false) {
    freq_[notes_] = round(freq);
    durations_[notes_] = round(duration);

    //delay duration should always be 1 ms more than the note in order to separate them.
    delays_[notes_ + 1] = (doubleDelay ? 2 * (durations_[notes_] + 1) : durations_[notes_] + 1);

    ++notes_;
}

void StarWars::InitNotes() {
    AddNote(LA3, Q);
    AddNote(LA3, Q);
    AddNote(LA3, Q);
    AddNote(F3,  E + S);
    AddNote(C4,  S);

    AddNote(LA3, Q);
    AddNote(F3,  E + S);
    AddNote(C4,  S);
    AddNote(LA3, H);

    AddNote(E4,  Q);
    AddNote(E4,  Q);
    AddNote(E4,  Q);
    AddNote(F4,  E + S);
    AddNote(C4,  S);

    AddNote(Ab3, Q);
    AddNote(F3,  E + S);
    AddNote(C4,  S);
    AddNote(LA3, H);

    AddNote(LA4, Q);
    AddNote(LA3, E + S);
    AddNote(LA3, S);
    AddNote(LA4, Q);
    AddNote(Ab4, E + S);
    AddNote(G4,  S);

    AddNote(Gb4, S);
    AddNote(E4,  S);
    AddNote(F4,  E, true);//PAUSE
    AddNote(Bb3, E);
    AddNote(Eb4, Q);
    AddNote(D4,  E + S);
    AddNote(Db4, S);

    AddNote(C4,  S);
    AddNote(B3,  S);
    AddNote(C4,  E, true);//PAUSE QUASI FINE RIGA
    AddNote(F3,  E);
    AddNote(Ab3, Q);
    AddNote(F3,  E + S);
    AddNote(LA3, S);

    AddNote(C4,  Q);
    AddNote(LA3, E + S);
    AddNote(C4,  S);
    AddNote(E4,  H);

    AddNote(LA4, Q);
    AddNote(LA3, E + S);
    AddNote(LA3, S);
    AddNote(LA4, Q);
    AddNote(Ab4, E + S);
    AddNote(G4,  S);

    AddNote(Gb4, S);
    AddNote(E4,  S);
    AddNote(F4,  E, true);//PAUSE
    AddNote(Bb3, E);
    AddNote(Eb4, Q);
    AddNote(D4,  E + S);
    AddNote(Db4, S);

    AddNote(C4,  S);
    AddNote(B3,  S);
    AddNote(C4,  E, true);//PAUSE QUASI FINE RIGA
    AddNote(F3,  E);
    AddNote(Ab3, Q);
    AddNote(F3,  E + S);
    AddNote(C4,  S);

    AddNote(LA3, Q);
    AddNote(F3,  E + S);
    AddNote(C4,  S);
    AddNote(LA3, H);

    // Pause after each play
    delays_[0] = 2 * H;
}

void StarWars::PlayNotes() {
    auto currentMillis = millis();
    if (currentMillis - audioStartMillis_ >= delays_[currNote_] && currNote_ < notes_) {
        audioStartMillis_ = currentMillis;

        tone(buzzerPin_, freq_[currNote_], durations_[currNote_]);

        ++currNote_;
    }

    if (currNote_ == notes_ && currentMillis - audioStartMillis_ >= delays_[0]) {
        currNote_ = 0;
    }
}

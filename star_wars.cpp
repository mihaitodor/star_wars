#include <Arduino.h>

#include "star_wars.h"

namespace {
    char* myStrings[]={
        "It is a period of civil war.",
        "Rebel spaceships, striking",
        "from a hidden base, have won",
        "their first victory against",
        "the evil Galactic Empire.",
        "",
        "During the battle, Rebel",
        "spies managed to steal secret",
        "plans to the Empire's",
        "ultimate weapon, the DEATH",
        "STAR, an armored space",
        "station with enough power",
        "to destroy an entire planet.",
        "",
        "Pursued by the Empire's",
        "sinister agents, Princess",
        "Leia races home aboard her",
        "starship, custodian of the",
        "stolen plans that can save her",
        "people and restore",
        "freedom to the galaxy..."
    };
}

/*
 Demonstrates the use a 16x2 LCD display. The LiquidCrystal
 library works with all LCD displays that are compatible with the
 Hitachi HD44780 driver. There are many of them out there, and you
 can usually tell them by the 16-pin interface.

 The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)
*/
StarWars::StarWars() :
    //                 rs, en, d0, d1, d2, d3, d4, d5, d6, d7
    lcd_(LiquidCrystal(12, 11,  9,  8,  7,  6,  5,  4,  3,  2)),
    // Use pin 0 for the buzzer
    timers_(0) {

    // Set up the LCD's number of columns and rows
    lcd_.begin(16, 2);
}

StarWars &StarWars::GetInstance() {
    static StarWars sw;

    return sw;
}

void StarWars::PrintTitle() {
    lcd_.setCursor(6, 0);
    lcd_.print("STAR");
    lcd_.setCursor(6, 1);
    lcd_.print("WARS");
}

void StarWars::FadeTitle() {
    static unsigned long iterations = 0;

    iterations++;

    //if (iterations > 300000) {
        lcd_.setCursor(6, 1);
        lcd_.blink();
        lcd_.setCursor(7, 1);
        lcd_.blink();
        lcd_.setCursor(8, 1);
        lcd_.blink();
        lcd_.setCursor(9, 1);
        lcd_.blink();
    //}
}

// Source for audio: http://www.instructables.com/id/How-to-easily-play-music-with-buzzer-on-arduino-Th/
// More details: https://github.com/bhagman/Tone

// NB: ALL NOTES DEFINED WITH STANDARD ENGLISH NAMES, EXCEPT FROM "A"
//THAT IS CALLED WITH THE ITALIAN NAME "LA" BECAUSE A0,A1...ARE THE ANALOG PINS ON ARDUINO.
// (Ab IS CALLED Ab AND NOT LAb)
#define C0 16.35
#define Db0 17.32
#define D0  18.35
#define Eb0 19.45
#define E0  20.60
#define F0  21.83
#define Gb0 23.12
#define G0  24.50
#define Ab0 25.96
#define LA0 27.50
#define Bb0 29.14
#define B0  30.87
#define C1  32.70
#define Db1 34.65
#define D1  36.71
#define Eb1 38.89
#define E1  41.20
#define F1  43.65
#define Gb1 46.25
#define G1  49.00
#define Ab1 51.91
#define LA1 55.00
#define Bb1 58.27
#define B1  61.74
#define C2  65.41
#define Db2 69.30
#define D2  73.42
#define Eb2 77.78
#define E2  82.41
#define F2  87.31
#define Gb2 92.50
#define G2  98.00
#define Ab2 103.83
#define LA2 110.00
#define Bb2 116.54
#define B2  123.47
#define C3  130.81
#define Db3 138.59
#define D3  146.83
#define Eb3 155.56
#define E3  164.81
#define F3  174.61
#define Gb3 185.00
#define G3  196.00
#define Ab3 207.65
#define LA3 220.00
#define Bb3 233.08
#define B3  246.94
#define C4  261.63
#define Db4 277.18
#define D4  293.66
#define Eb4 311.13
#define E4  329.63
#define F4  349.23
#define Gb4 369.99
#define G4  392.00
#define Ab4 415.30
#define LA4 440.00
#define Bb4 466.16
#define B4  493.88
#define C5  523.25
#define Db5 554.37
#define D5  587.33
#define Eb5 622.25
#define E5  659.26
#define F5  698.46
#define Gb5 739.99
#define G5  783.99
#define Ab5 830.61
#define LA5 880.00
#define Bb5 932.33
#define B5  987.77
#define C6  1046.50
#define Db6 1108.73
#define D6  1174.66
#define Eb6 1244.51
#define E6  1318.51
#define F6  1396.91
#define Gb6 1479.98
#define G6  1567.98
#define Ab6 1661.22
#define LA6 1760.00
#define Bb6 1864.66
#define B6  1975.53
#define C7  2093.00
#define Db7 2217.46
#define D7  2349.32
#define Eb7 2489.02
#define E7  2637.02
#define F7  2793.83
#define Gb7 2959.96
#define G7  3135.96
#define Ab7 3322.44
#define LA7 3520.01
#define Bb7 3729.31
#define B7  3951.07
#define C8  4186.01
#define Db8 4434.92
#define D8  4698.64
#define Eb8 4978.03
// DURATION OF THE NOTES
#define BPM 120    //  you can change this value changing all the others
#define H 2*Q //half 2/4
#define Q 60000/BPM //quarter 1/4
#define E Q/2   //eighth 1/8
#define S Q/4 // sixteenth 1/16
#define W 4*Q // whole 4/4

#define TP 0



Timers::Timers(uint8_t pin) :
    pin_(pin),
    notes_{},
    freq_{},
    durations_{},
    doubleDelays_{},
    millis_{},
    totalMillis_(0) {
}

void Timers::Add(uint8_t freq, uint8_t duration, bool doubleDelay = false) {
    freq_[notes_] = freq;
    durations_[notes_] = duration;
    doubleDelays_[notes_] = doubleDelay; //TODO: Remove member & refactor

    if (notes_ == 0) {
        millis_[notes_] = duration;
    } else {
        millis_[notes_] = millis_[notes_ - 1] + duration;
    }

    ++notes_;
}

void StarWars::InitAudio() {
    Timers t(TP);

    t.Add(LA3,Q);
    t.Add(LA3,Q);
    t.Add(LA3,Q);
    t.Add(F3,E+S);
    t.Add(C4,S);

    t.Add(LA3,Q);
    t.Add(F3,E+S);
    t.Add(C4,S);
    t.Add(LA3,H);

    t.Add(E4,Q);
    t.Add(E4,Q);
    t.Add(E4,Q);
    t.Add(F4,E+S);
    t.Add(C4,S);

    t.Add(Ab3,Q);
    t.Add(F3,E+S);
    t.Add(C4,S);
    t.Add(LA3,H);
}

void StarWars::PlayAudio() {
    for (uint8_t note = 0; note < notes_; ++ note) {
        unsigned long lastUpdate = millis();
    }

}

void PlayAudio_old() {
    //delay duration should always be 1 ms more than the note in order to separate them.
    //tone(pin, note, duration)
    tone(TP,LA3,Q);
    delay(1+Q);
    tone(TP,LA3,Q);
    delay(1+Q);
    tone(TP,LA3,Q);
    delay(1+Q);
    tone(TP,F3,E+S);
    delay(1+E+S);
    tone(TP,C4,S);
    delay(1+S);

    tone(TP,LA3,Q);
    delay(1+Q);
    tone(TP,F3,E+S);
    delay(1+E+S);
    tone(TP,C4,S);
    delay(1+S);
    tone(TP,LA3,H);
    delay(1+H);

    tone(TP,E4,Q);
    delay(1+Q);
    tone(TP,E4,Q);
    delay(1+Q);
    tone(TP,E4,Q);
    delay(1+Q);
    tone(TP,F4,E+S);
    delay(1+E+S);
    tone(TP,C4,S);
    delay(1+S);

    tone(TP,Ab3,Q);
    delay(1+Q);
    tone(TP,F3,E+S);
    delay(1+E+S);
    tone(TP,C4,S);
    delay(1+S);
    tone(TP,LA3,H);
    delay(1+H);

    tone(TP,LA4,Q);
    delay(1+Q);
    tone(TP,LA3,E+S);
    delay(1+E+S);
    tone(TP,LA3,S);
    delay(1+S);
    tone(TP,LA4,Q);
    delay(1+Q);
    tone(TP,Ab4,E+S);
    delay(1+E+S);
    tone(TP,G4,S);
    delay(1+S);

    tone(TP,Gb4,S);
    delay(1+S);
    tone(TP,E4,S);
    delay(1+S);
    tone(TP,F4,E);
    delay(1+E);
    delay(1+E);//PAUSE
    tone(TP,Bb3,E);
    delay(1+E);
    tone(TP,Eb4,Q);
    delay(1+Q);
    tone(TP,D4,E+S);
    delay(1+E+S);
    tone(TP,Db4,S);
    delay(1+S);

    tone(TP,C4,S);
    delay(1+S);
    tone(TP,B3,S);
    delay(1+S);
    tone(TP,C4,E);
    delay(1+E);
    delay(1+E);//PAUSE QUASI FINE RIGA
    tone(TP,F3,E);
    delay(1+E);
    tone(TP,Ab3,Q);
    delay(1+Q);
    tone(TP,F3,E+S);
    delay(1+E+S);
    tone(TP,LA3,S);
    delay(1+S);

    tone(TP,C4,Q);
    delay(1+Q);
    tone(TP,LA3,E+S);
    delay(1+E+S);
    tone(TP,C4,S);
    delay(1+S);
    tone(TP,E4,H);
    delay(1+H);

    tone(TP,LA4,Q);
    delay(1+Q);
    tone(TP,LA3,E+S);
    delay(1+E+S);
    tone(TP,LA3,S);
    delay(1+S);
    tone(TP,LA4,Q);
    delay(1+Q);
    tone(TP,Ab4,E+S);
    delay(1+E+S);
    tone(TP,G4,S);
    delay(1+S);

    tone(TP,Gb4,S);
    delay(1+S);
    tone(TP,E4,S);
    delay(1+S);
    tone(TP,F4,E);
    delay(1+E);
    delay(1+E);//PAUSE
    tone(TP,Bb3,E);
    delay(1+E);
    tone(TP,Eb4,Q);
    delay(1+Q);
    tone(TP,D4,E+S);
    delay(1+E+S);
    tone(TP,Db4,S);
    delay(1+S);

    tone(TP,C4,S);
    delay(1+S);
    tone(TP,B3,S);
    delay(1+S);
    tone(TP,C4,E);
    delay(1+E);
    delay(1+E);//PAUSE QUASI FINE RIGA
    tone(TP,F3,E);
    delay(1+E);
    tone(TP,Ab3,Q);
    delay(1+Q);
    tone(TP,F3,E+S);
    delay(1+E+S);
    tone(TP,C4,S);
    delay(1+S);

    tone(TP,LA3,Q);
    delay(1+Q);
    tone(TP,F3,E+S);
    delay(1+E+S);
    tone(TP,C4,S);
    delay(1+S);
    tone(TP,LA3,H);
    delay(1+H);

    delay(2*H);
}

#include "star_wars.h"

#include <LiquidCrystal.h>

void setup() {
    // Serial.begin(9600);

    auto &sw = StarWars::GetInstance();

    sw.PrintTitle();

    sw.InitNotes();
}

// Multitasking tutorial: https://learn.adafruit.com/multi-tasking-the-arduino-part-1/overview?view=all
void loop() {
    auto &sw = StarWars::GetInstance();

    sw.PlayPause();

    if (!sw.IsRunning()) {
        return;
    }

    sw.PlayNotes();

    sw.BlinkTitle();

    sw.ShowOpeningCrawl();

    sw.ShowEnding();
}

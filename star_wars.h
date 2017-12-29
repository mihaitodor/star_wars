#ifndef STAR_WARS_H
#define STAR_WARS_H

#include <LiquidCrystal.h>

class Timers {
public:
    explicit Timers(uint8_t pin);
    void Add();

private:
    uint8_t pin_;
    uint8_t notes_;
    uint8_t freq_[72];
    uint8_t durations_[72];
    bool doubleDelays_[72];
    unsigned long millis_[72];
    unsigned long totalMillis_;
};

class StarWars {
public:
    static StarWars &GetInstance();

    void PrintTitle();

    void FadeTitle();

    static void PlayAudio();

private:
    StarWars();

    LiquidCrystal lcd_;
    Timers timers_;
    static StarWars instance_;
};

#endif //STAR_WARS_H

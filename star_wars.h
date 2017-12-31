#ifndef STAR_WARS_H
#define STAR_WARS_H

#include <LiquidCrystal.h>

class StarWars {
public:
    static const StarWars &GetInstance();

    void PrintTitle();
    //void FadeTitle();

    void AddNote(float freq, float duration, bool doubleDelay);
    void InitNotes();
    void PlayNotes();

    void Loop();

private:
    StarWars();
    StarWars(const StarWars &) = delete;
    StarWars(StarWars &&) = delete;

    LiquidCrystal lcd_;
    static StarWars instance_;

    uint8_t buzzerPin_;

    uint8_t currNote_ = 0;
    unsigned long startMillis_ = 0;

    uint8_t notes_ = 0;
    unsigned int freq_[66] {};
    unsigned long durations_[66] {};
    unsigned long delays_[66 + 1] {};

    unsigned long pauseDuration_ = 0;
};

#endif //STAR_WARS_H

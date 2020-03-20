// Blinker.hpp
// Callum Howard, 2020

#ifndef BLINKER_HPP
#define BLINKER_HPP

namespace ch {

class Blinker {
public:
    Blinker(const int led) : mLed{led} {
        pinMode(mLed, OUTPUT);
    }
    void update() {
        //loop to blink without delay
        unsigned long currentMillis = millis();

        if (currentMillis - mPreviousMillis >= mInterval) {
            // save the last time you blinked the LED
            mPreviousMillis = currentMillis;

            // if the LED is off turn it on and vice-versa:
            mLedState = not mLedState;

            // set the LED with the ledState of the variable:
            digitalWrite(mLed, mLedState);
        }
    }

private:
    const int mLed;
    unsigned long mPreviousMillis = 0;        // will store last time LED was updated
    const long mInterval = 500;           // interval at which to blink (milliseconds)
    int mLedState = LOW;             // ledState used to set the LED
};

} // namespace ch

#endif

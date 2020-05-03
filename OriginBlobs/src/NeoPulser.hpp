// NeoPulser.hpp
// Callum Howard, 2020

#ifndef NEOPULSER_HPP
#define NEOPULSER_HPP

#include <Adafruit_NeoPixel.h>

#define NEOPIXEL_PIN 21
#define NUM_PIXELS 1

namespace ch {

class NeoPulser {
public:
    NeoPulser() {
        mPixels = Adafruit_NeoPixel(NUM_PIXELS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);
        mPixels.begin();
    }

    void update() {
        //loop to blink without delay
        unsigned long currentMillis = millis();

        if (currentMillis - mPreviousMillis >= mInterval) {
            // save the last time you blinked the LED
            mPreviousMillis = currentMillis;

            mIsOn = !mIsOn;
            if (mIsOn) {
                mPixels.setPixelColor(0, mPixels.Color(0, 127, 255));
            } else {
                mPixels.setPixelColor(0, mPixels.Color(0, 0, 0));
            }
            mPixels.show();
        }
    }

private:
    unsigned long mPreviousMillis = 0;          // will store last time LED was updated
    const unsigned long mInterval = 3000;        // interval at which to blink (milliseconds)
    bool mIsOn = true;

    Adafruit_NeoPixel mPixels;
};

} // namespace ch

#endif

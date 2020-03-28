// Pulser.hpp
// Callum Howard, 2020

#ifndef BLINKER_HPP
#define BLINKER_HPP

#include <array>
#include <cmath>

#include "Utils.hpp"  // bezierPoint, lerp, log


namespace ch {

class Pulser {
public:
    Pulser(const uint8_t led) : mLedPin{led} {
        // Setup timer and attach timer to a led pin
        ledcSetup(mLedChannel, mLedBaseFreq, mTimerResolution);
        ledcAttachPin(mLedPin, mLedChannel);
        mDutyFactor = (1 << mTimerResolution) / mMaxBrightness;
    }

    // Arduino like analogWrite
    // value has to be between 0 and valueMax
    void ledcAnalogWrite(uint8_t channel, uint32_t value) {
        const uint32_t duty =
                mDutyFactor * min(value, static_cast<uint32_t>(mMaxBrightness));

        // write duty to LEDC
        ledcWrite(mLedChannel, duty);
    }

    void update() {
        //loop to blink without delay
        unsigned long currentMillis = millis();

        if (currentMillis - mPreviousMillis >= mInterval) {
            // save the last time you blinked the LED
            mPreviousMillis = currentMillis;

            // change the brightness for next time through the loop:
            const unsigned long elapsed = (currentMillis - mTriggeredMillis);
            if (elapsed < mDuration) {
                const float t = static_cast<float>(elapsed) / mDuration;
                mBrightness = lerp(mMaxBrightness, static_cast<uint8_t>(0), bezierPoint(xs, t));
            } else {
                mBrightness = 0;
            }

            // set the LED with the ledState of the variable:
            ledcAnalogWrite(mLedPin, mBrightness);
        }
    }

    void trigger() {
        mTriggeredMillis = millis();
    }

private:
    const uint8_t mLedPin;
    const uint8_t mLedChannel = 0;              // specify channel from 0 to 15
    const uint16_t mLedBaseFreq = 5000;
    const uint8_t mTimerResolution = 13;        // from 0 to 255
    const uint8_t mMaxBrightness = 255;
    const unsigned long mDuration = 200;
    const unsigned long mInterval = 30;        // interval at which to blink (milliseconds)

    uint32_t mDutyFactor;
    unsigned long mTriggeredMillis = 0;          // will store last time triggered
    unsigned long mPreviousMillis = 0;          // will store last time LED was updated
    uint8_t mBrightness = 0;                    // ledState used to set the LED
    uint8_t mFadeAmount = 5;

    // Bezier animation curve
    const std::array<float, 4> xs = std::array<float, 4>{0.0f, 0.22f, 0.36f, 1.0f};
    //static constexpr std::array<float, 4> ys{0, 1.0, 1.0, 1.0};

    ch::Logger log;
};

} // namespace ch

#endif

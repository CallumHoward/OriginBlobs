// Pulser.hpp
// Callum Howard, 2020

#ifndef BLINKER_HPP
#define BLINKER_HPP

#include "Utils.hpp"

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
            mBrightness = mBrightness + mFadeAmount;

            // reverse the direction of the fading at the ends of the fade:
            if (mBrightness <= 0 || mBrightness >= mMaxBrightness) {
                mFadeAmount = -mFadeAmount;
            }

            // set the LED with the ledState of the variable:
            ledcAnalogWrite(mLedPin, mBrightness);
        }
    }

private:
    const uint8_t mLedPin;
    const uint8_t mLedChannel = 0;              // specify channel from 0 to 15
    const uint16_t mLedBaseFreq = 5000;
    const uint8_t mTimerResolution = 13;        // from 0 to 255
    const uint8_t mMaxBrightness = 255;
    const unsigned long mInterval = 30;        // interval at which to blink (milliseconds)
    uint32_t mDutyFactor;
    bool mTriggered = false;  //TODO include this
    unsigned long mPreviousMillis = 0;          // will store last time LED was updated
    uint8_t mBrightness = 0;                    // ledState used to set the LED
    uint8_t mFadeAmount = 5;
};

} // namespace ch

#endif

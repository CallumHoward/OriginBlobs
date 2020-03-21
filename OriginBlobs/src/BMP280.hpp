// BMP280.hpp
// Callum Howard, 2020

#ifndef BMP280_HPP
#define BMP280_HPP

#include <deque>
#include <functional>  // bind
#include <iterator>  // back_inserter

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BMP280.h>

#include "Utils.hpp"

namespace ch {

class BMP280 {
public:
    BMP280(const int bufferSize = 32) : mBufferSize{bufferSize} {
        log.info(F("Setting up BMP280..."));
        if (not mBmp.begin(BMP280_ADDRESS_ALT)) {
            log.info(F("\nCould not find a valid BMP280 sensor, check wiring!\n"));
        } else {
            log.info(F("done!"));
        }

        /* Default settings from datasheet. */
        mBmp.setSampling(
                Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                Adafruit_BMP280::FILTER_OFF,      /* Filtering. */
                Adafruit_BMP280::STANDBY_MS_1); /* Standby time. */

        std::generate_n(
                std::back_inserter(mPressureBuffer),
                bufferSize,
                std::bind(&Adafruit_BMP280::readPressure, &mBmp));

        mFloatingAverage = std::accumulate(
                std::begin(mPressureBuffer),
                std::end(mPressureBuffer), 0);
    }

    void update() {

        unsigned long currentMillis = millis();
        if (currentMillis - mPreviousMillis >= mInterval) {
            mPreviousMillis = currentMillis;

            const float currentPressure = mBmp.readPressure();

            // take from deque
            mFloatingAverage -= mPressureBuffer.front();
            mPressureBuffer.pop_front();
            mPressureBuffer.push_back(currentPressure);
            mFloatingAverage += currentPressure;

            log.info(F("Pressure "));
            log.info(currentPressure);
            log.info(F(","));

            log.info(F("Average "));
            log.info(mFloatingAverage / mBufferSize);
            log.info(F("\n"));

            if (currentPressure > (mFloatingAverage / mBufferSize)  + mThreshold) {
                log.info(F("Pressure triggered!"));
                log.info(F("\n"));
            }
            mActivationCallback();
        }
    }

    void setActivationCallback(const std::function<void()>& activationCallback) {
        log.info(F("Set Activation Callback\n"));
        mActivationCallback = activationCallback;
    }

private:
    ch::Logger log;
    unsigned long mPreviousMillis = 0;        // will store last time readings were printed
    const long mInterval = 1250;           // interval at which to blink (milliseconds)
    float lastPressure = 0;
    const float mThreshold = 35;

    float mFloatingAverage = 0;  // not true average, must divide when used
    const int mBufferSize = 0;
    std::deque<float> mPressureBuffer;
    std::function<void()> mActivationCallback;

    Adafruit_BMP280 mBmp; // I2C
    //Adafruit_BMP280 mBmp(BMP_CS); // hardware SPI
    //Adafruit_BMP280 mBmp(BMP_CS, BMP_MOSI, BMP_MISO,  BMP_SCK);
};

} // namespace ch

#endif

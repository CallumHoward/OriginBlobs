// BLEScanner.hpp
// Callum Howard, 2020

/*
   Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleScan.cpp
   Ported to Arduino ESP32 by Evandro Copercini
*/

#ifndef BLESCANNER_HPP
#define BLESCANNER_HPP

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

#include "Utils.hpp"  // Logger

namespace ch {

class BLEScanner {
public:
    void setup() {
        log.info("Scanning...\n");

        BLEDevice::init("");
        mpBLEScan = BLEDevice::getScan(); //create new scan
        mpBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
        mpBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
        mpBLEScan->setInterval(100);
        mpBLEScan->setWindow(99);  // less or equal setInterval value
    }

    void update() {
        const unsigned long currentMillis = millis();
        if (currentMillis - mPreviousMillis >= mInterval) {
            // reset timeout
            mPreviousMillis = currentMillis;

            // perform scan
            BLEScanResults foundDevices = mpBLEScan->start(mScanTime, false);
            log.info("Devices found: ");
            log.info(String(foundDevices.getCount()));
            log.info("\nScan done!\n");
            mpBLEScan->clearResults();   // delete results fromBLEScan buffer to release memory
        }
    }

private:
    class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    public:
        void onResult(BLEAdvertisedDevice advertisedDevice) {
            log.info(String("Advertised Device: ") +
                    advertisedDevice.toString().c_str() +
                    " \tRSSI: " +
                    String(advertisedDevice.getRSSI()) +
                    "\n");
        }

    private:
        Logger log;
    };

    Logger log;
    unsigned long mPreviousMillis = 0;
    const long mInterval = 2000;

    int mScanTime = 5;  // in seconds
    BLEScan* mpBLEScan;
};

} // namespace ch

#endif

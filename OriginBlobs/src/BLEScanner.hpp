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
#include <BLEBeacon.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

#define BEACON_UUID           "8ec76ea3-6668-48da-9866-75be8bc86f4d" // UUID 1 128-Bit (may use linux tool uuidgen or random numbers via https://www.uuidgenerator.net/)

#include "Utils.hpp"  // Logger

namespace ch {

class BLEScanner {
public:
    void setup(const char* name = "") {
        log.info("Scanning...\n");

        BLEDevice::init(name);

        pAdvertising = BLEDevice::getAdvertising();
        setBeacon();
        pAdvertising->start();

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

    void setBeacon() {
        BLEBeacon oBeacon = BLEBeacon();
        oBeacon.setManufacturerId(0x4C00); // fake Apple 0x004C LSB (ENDIAN_CHANGE_U16!)
        oBeacon.setProximityUUID(BLEUUID(BEACON_UUID));
        const uint32_t bootcount = 0;
        oBeacon.setMajor((bootcount & 0xFFFF0000) >> 16);
        oBeacon.setMinor(bootcount&0xFFFF);
        BLEAdvertisementData oAdvertisementData = BLEAdvertisementData();
        BLEAdvertisementData oScanResponseData = BLEAdvertisementData();

        oAdvertisementData.setFlags(0x04); // BR_EDR_NOT_SUPPORTED 0x04

        std::string strServiceData = "";

        strServiceData += (char)26;     // Len
        strServiceData += (char)0xFF;   // Type
        strServiceData += oBeacon.getData(); 
        oAdvertisementData.addData(strServiceData);

        pAdvertising->setAdvertisementData(oAdvertisementData);
        pAdvertising->setScanResponseData(oScanResponseData);
    }

    Logger log;
    unsigned long mPreviousMillis = 0;
    const long mInterval = 2000;

    int mScanTime = 5;  // in seconds
    BLEScan* mpBLEScan;
    BLEAdvertising* pAdvertising;
};

} // namespace ch

#endif

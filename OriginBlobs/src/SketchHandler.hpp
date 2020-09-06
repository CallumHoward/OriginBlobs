// SketchHandler.hpp
// Callum Howard, 2020

#ifndef SKETCHHANDLER_HPP
#define SKETCHHANDLER_HPP

#define BOARD_NAME "esp32_stacey"

#include <functional>  // bind

#include "OTAHandler.hpp"
//#include "Pulser.hpp"
#include "NeoPulser.hpp"
#include "BLEScanner.hpp"
#include "MQTTHandler.hpp"
#include "BMP280.hpp"
#include "Credentials.hpp"

namespace ch {

class SketchHandler {
public:
    SketchHandler() :
        //pulser{2},
        bmp280{},
        neoPulser{},
        ota{BOARD_NAME, ssid, password, 80},
        mqttHandler{[](){ Serial.println("Received pulse"); }}
    {
        ota.begin();
        //scanner.setup(BOARD_NAME);
        //bmp280.subscribeToActivation(std::bind(&ch::Pulser::trigger, &pulser));
        //bmp280.subscribeToActivation(
        //        std::bind(&ch::MQTTHandler::notifyPulseSoulMate, &mqttHandler));
    }

    void update() {
        ota.update();
        delay(1);
        //pulser.update();
        bmp280.update();
        neoPulser.update();
        //scanner.update();
        mqttHandler.update();
    }

private:
    //ch::Pulser pulser;
    ch::BMP280 bmp280;
    ch::NeoPulser neoPulser;
    ch::OTAHandler ota;
    // ch::BLEScanner scanner;
    ch::MQTTHandler mqttHandler;
};


} // namespace ch

#endif

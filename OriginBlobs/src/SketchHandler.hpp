// SketchHandler.hpp
// Callum Howard, 2020

#ifndef SKETCHHANDLER_HPP
#define SKETCHHANDLER_HPP

#define BOARD_NAME "esp32_stacey"

#include "OTAHandler.hpp"
#include "Pulser.hpp"
#include "BLEScanner.hpp"
#include "MQTTHandler.hpp"
#include "BMP280.hpp"
#include "Credentials.hpp"

namespace ch {

class SketchHandler {
public:
    SketchHandler() :
        pulser{2},
        bmp280{},
        ota{BOARD_NAME, ssid, password, 80},
        mqttHandler{std::bind(&ch::Pulser::trigger, &pulser)}
    {
        ota.begin();
        //scanner.setup(BOARD_NAME);
        bmp280.subscribeToActivation(std::bind(&ch::Pulser::trigger, &pulser));
        bmp280.subscribeToActivation(
                std::bind(&ch::MQTTHandler::notifyPulseSoulMate, &mqttHandler));
    }

    void update() {
        ota.update();
        delay(1);
        pulser.update();
        bmp280.update();
        //scanner.update();
        mqttHandler.update();
    }

private:
    ch::Pulser pulser;
    ch::BMP280 bmp280;
    ch::OTAHandler ota;
    //ch::BLEScanner scanner;
    ch::MQTTHandler mqttHandler;
};


} // namespace ch

#endif

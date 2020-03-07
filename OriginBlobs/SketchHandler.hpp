// SketchHandler.hpp
// Callum Howard, 2020

#ifndef SKETCHHANDLER_HPP
#define SKETCHHANDLER_HPP

#include "OTAHandler.hpp"
#include "Blinker.hpp"
#include "BLEScanner.hpp"
#include "Credentials.hpp"

namespace ch {

class SketchHandler {
public:
    SketchHandler() :
        blinker{2},
        ota{"esp32", ssid, password, 80} {
        ota.begin();
        scanner.setup();
    }

    void update() {
        ota.update();
        delay(1);
        blinker.update();
        scanner.update();
    }

private:
    ch::Blinker blinker;
    ch::OTAHandler ota;
    ch::BLEScanner scanner;
};


} // namespace ch

#endif

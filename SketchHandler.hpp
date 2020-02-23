// SketchHandler.hpp
// Callum Howard, 2020

#ifndef SKETCHHANDLER_HPP
#define SKETCHHANDLER_HPP

#include "OTAHandler.hpp"
#include "Blinker.hpp"
#include "Credentials.hpp"

namespace ch {

class SketchHandler {
public:
    SketchHandler() :
        blinker{2},
        ota{"esp32", ssid, password, 80} {
        ota.begin();
    }

    void update() {
        ota.update();
        delay(1);
        blinker.update();
    }

private:
    ch::OTAHandler ota;
    ch::Blinker blinker;
};


} // namespace ch

#endif

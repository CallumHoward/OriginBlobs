#ifndef UTILS_HPP
#define UTILS_HPP

namespace ch {

class Logger {
public:
    void info(const String& message) {
        if (not sInitialised) {
            Serial.begin(sBaud);
            sInitialised = true;
        }
        Serial.print(message);
    }

    void info(const int value) {
        if (not sInitialised) {
            Serial.begin(sBaud);
            sInitialised = true;
        }
        Serial.print(value);
    }

    void info(const float value) {
        if (not sInitialised) {
            Serial.begin(sBaud);
            sInitialised = true;
        }
        Serial.print(value);
    }

private:
    const int sBaud = 115200;
    bool sInitialised = false;
};

} // namespace ch

#endif

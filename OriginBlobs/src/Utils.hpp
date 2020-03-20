#ifndef UTILS_HPP
#define UTILS_HPP

#include <Array>
#include <Utility>

namespace ch {

// return point on given cubic bezier curve given u in range [0, 1]
inline std::pair<float, float> bezierCurve(const std::array<float, 4>& xs, const std::array<float, 4>& ys, const float u) {
    return std::pair<float, float>{
        pow(1 - u, 3) * xs[0] +
        3 * u * pow(1 - u, 2) * xs[1] +
        3 * pow(u, 2) * (1 - u) * xs[2] +
        pow(u, 3) * xs[3],
        pow(1 - u, 3) * ys[0] +
        3 * u * pow(1 - u, 2) * ys[1] +
        3 * pow(u, 2) * (1 - u) * ys[2] +
        pow(u, 3) * ys[3]
    };
}

class Logger {
public:
    void info(const String &message) {
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

}  // namespace ch

#endif

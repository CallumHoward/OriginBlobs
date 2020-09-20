#ifndef UTILS_HPP
#define UTILS_HPP

#ifdef TEST
#include <cmath>
#endif

#include <array>
#include <utility>  // pair

namespace ch {

// return value on given cubic bezier curve given t in range [0, 1]
inline float bezierPoint(const std::array<float, 4>& v, const float t) {
    const float t1 = 1.0f - t;
    return (v[0] * t1 + 3 * v[1] * t) * t1 * t1 + (3 * v[2] * t1 + v[3] * t) * t * t;
}

template <typename T, typename L>
T lerp(const T &a, const T &b, L factor) {
    return a + (b - a) * factor;
}

#ifndef TEST
class Logger {
public:
    void info(const String &message) const {
        if (not sInitialised) {
            Serial.begin(sBaud);
            sInitialised = true;
        }
        Serial.print(message);
    }

    void info(const unsigned int value) const {
        if (not sInitialised) {
            Serial.begin(sBaud);
            sInitialised = true;
        }
        Serial.print(value);
    }

    void info(const int value) const {
        if (not sInitialised) {
            Serial.begin(sBaud);
            sInitialised = true;
        }
        Serial.print(value);
    }

    void info(const float value) const {
        if (not sInitialised) {
            Serial.begin(sBaud);
            sInitialised = true;
        }
        Serial.print(value);
    }

private:
    const int sBaud = 115200;
    mutable bool sInitialised = false;
};
#endif

}  // namespace ch

#endif

// testBezier.cpp
// Callum Howard, 2020

#include <array>
#include <iostream>

#include "bezier.h"
#include "Utils.hpp"


int main() {

    // Bezier animation curve
    const std::array<float, 4> xs{0.0f, 0.22f, 0.36f, 1.0f};
    //const std::array<float, 4> ys{0.0f, 1.0f, 1.0f, 1.0f};

    for (const auto i : {0.0f, 0.25f, 0.5f, 0.75f, 1.0f}) {
        //const auto result = ch::bezierCurve(xs, i);
        const float result = ch::bezierPoint(xs, i);
        std::cout << result << '\n';
    }

    std::cout << '\n';

    Bezier::Bezier<3> cubicBezier({
            {0.0f, 0.0f},
            {0.22f, 1.0f},
            {0.36f, 1.0f},
            {1.0f, 1.0f}
            });

    //Bezier::Bezier<3> cubicBezier({ {120, 160}, {35, 200}, {220, 260}, {220, 40} });

    for (const auto i : {0.0f, 0.25f, 0.5f, 0.75f, 1.0f}) {
        static constexpr int X_AXIS = 0;
        std::cout << cubicBezier.valueAt(i, X_AXIS) << '\n';
    }

    //const float i = 0.3141592;
    //const auto result = ch::bezierCurve(xs, i);
    //std::cout << result << '\n';

    //static constexpr int X_AXIS = 0;
    //std::cout << cubicBezier.valueAt(i, X_AXIS) << '\n';

    return 0;
}

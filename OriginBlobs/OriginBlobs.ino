// OriginBlobs.ino
// Callum Howard, 2020

#include "src/SketchHandler.hpp"

ch::SketchHandler* sketch;

void setup(void) {
    sketch = new ch::SketchHandler{};
}

void loop(void) {
    sketch->update();
}

#include <Arduino.h>
#include "../lib/MultiColorLed/src/MultiColorLed.h"

MultiColorLed multiColorLed;

__attribute__((unused)) void setup() {
    multiColorLed.init();
}

__attribute__((unused)) void loop() {
    multiColorLed.run();
}


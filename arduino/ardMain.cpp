//
// Created by Westly Bouchard on 3/2/26.
//

#include "../ArduinoRuntime.h"

#include "Motor.h"

Motor FL(9);
Motor FR(10);
Motor BL(11);
Motor BR(12);

void setup() {}

int counter = 0;

void loop() {
    if (counter < 1000) {
        FL.run(175);
        BL.run(175);
        FR.run(0);
        BR.run(0);
    } else if (counter < 2000) {
        FL.run(0);
        BL.run(0);
        FR.run(175);
        BR.run(175);
    } else {
        FL.run(0);
        BL.run(0);
        FR.run(0);
        BR.run(0);
    }
    counter++;
}
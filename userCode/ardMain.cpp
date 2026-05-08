//
// Created by Westly Bouchard on 3/2/26.
//

#include "Drivetrain.h"

void setup() {
    pinMode(1, INPUT);
}

void loop() {
    if (digitalRead(1)) {
        FL_M.run(100);
        FR_M.run(100);
        BL_M.run(100);
        BR_M.run(100);
    } else {
        FL_M.run(0);
        FR_M.run(0);
        BL_M.run(0);
        BR_M.run(0);
    }
}
//
// Created by Westly Bouchard on 3/2/26.
//

#include "Drivetrain.h"

void setup() {
    delay(500);
}

void loop() {
    FL_M.run(-100);
    BL_M.run(-100);

    FR_M.run(100);
    BR_M.run(100);
}
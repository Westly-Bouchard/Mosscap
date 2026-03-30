//
// Created by Westly Bouchard on 3/2/26.
//

#include "../arduino/ArduinoRuntime.h"

#include "../arduino/Encoder.h"
#include "../arduino/Motor.h"

Motor FL(9);
Motor FR(10);
Motor BL(11);
Motor BR(12);

void move(const int pwm, const int time) {
    FL.run(pwm);
    FR.run(pwm);
    BL.run(pwm);
    BR.run(pwm);
    delay(time);

    FL.run(0);
    FR.run(0);
    BL.run(0);
    BR.run(0);
}

void setup() {}

void loop() {
    move(150, 500);

    delay(500);

    move(-150, 500);

    delay(500);
}
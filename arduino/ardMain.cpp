//
// Created by Westly Bouchard on 3/2/26.
//

#include "../ArduinoRuntime.h"

#include <iostream>

#include "Encoder.h"
#include "Motor.h"

Motor FL(9);
Motor FR(10);
Motor BL(11);
Motor BR(12);

Encoder test(20,21);

void setup() {}

int counter = 0;

void forward(const int pwm) {
    FL.run(pwm);
    FR.run(pwm);
    BL.run(pwm);
    BR.run(pwm);
}

void strafe(const int pwm) {
    FL.run(-pwm);
    FR.run(pwm);
    BL.run(pwm);
    BR.run(-pwm);
}

void turn(const int pwm) {
    FL.run(pwm);
    FR.run(-pwm);
    BL.run(-pwm);
    BR.run(pwm);
}

void loop() {
    if (millis() < 1000) {
        forward(150);
    } else {
        forward(0);
    }

    std::cout << "FL Encoder Count: " << test.read() << std::endl;

    counter++;
}
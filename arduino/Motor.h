//
// Created by Westly Bouchard on 3/2/26.
//

#ifndef MOTOR_H
#define MOTOR_H

#include "../ArduinoRuntime.h"

class Motor {
public:
    explicit Motor(const int pinA) : pin(pinA) {}

    void run(const int pwm) const {
        const ArduinoRuntime& instance = ArduinoRuntime::getInstance();
        const auto handle = instance.getPWM(pin);
        handle->writePWM(pwm);
        // ArduinoRuntime::getInstance().getPWM(pin)->writePWM(pwm);
    }

private:
    int pin;
    Handle<WriteablePWM> h;
};

#endif //MOTOR_H

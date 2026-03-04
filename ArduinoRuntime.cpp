//
// Created by Westly Bouchard on 3/2/26.
//

#include "ArduinoRuntime.h"

ArduinoRuntime &ArduinoRuntime::getInstance() {
    static ArduinoRuntime instance;
    return instance;
}

void ArduinoRuntime::bindPWM(const int pin, WriteablePWM &pwm) {
    pwmMap.emplace(pin, Handle{&pwm});
}

void ArduinoRuntime::bindEncoder(const int pin, ReadableEncoder &encoder) {
    encoderMap.emplace(pin, Handle{&encoder});
}

Handle<WriteablePWM> ArduinoRuntime::getPWM(const int pin) const {
    return pwmMap.at(pin);
}

Handle<ReadableEncoder> ArduinoRuntime::getEncoder(const int pin) const {
    return encoderMap.at(pin);
}
//
// Created by Westly Bouchard on 3/2/26.
//

#include "ArduinoRuntime.h"

#include "Simulator.h"

ArduinoRuntime::ArduinoRuntime() {
    // Grab clock handle directly from simulator so that the user doesn't have
    // to do it in Sim.h
    SimClock& c = Simulator::getClock();
    clock = Handle{&dynamic_cast<ReadableTime&>(c)};
}

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

Handle<ReadableTime> ArduinoRuntime::getClock() const {
    return clock;
}

/**
 * Arduino time functions
 */

/* Static cast to int is appropriate here because we only want to count whole
 * micro or milliseconds that have passed
 */

int millis() {
    return static_cast<int>(ArduinoRuntime::getInstance().getClock()->readTime() * 1'000);
}

int micros() {
    return static_cast<int>(ArduinoRuntime::getInstance().getClock()->readTime() * 1'000'000);
}
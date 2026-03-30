//
// Created by Westly Bouchard on 3/2/26.
//

#include "ArduinoRuntime.h"

#include <chrono>
#include <thread>

#include "../sim/SimulatorBase.h"

ArduinoRuntime::ArduinoRuntime() : clock(SimulatorBase::getClock()) {}

ArduinoRuntime &ArduinoRuntime::getInstance() {
    static ArduinoRuntime instance;
    return instance;
}

void ArduinoRuntime::bindPWM(const int pin, WriteablePWM &pwm) {
    pwmMap.emplace(pin, Handle{pwm});
}

void ArduinoRuntime::bindEncoder(const int pin, ReadableEncoder &encoder) {
    encoderMap.emplace(pin, Handle{encoder});
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
    return static_cast<int>(ArduinoRuntime::getInstance().getClock().get().readTime() * 1'000);
}

int micros() {
    return static_cast<int>(ArduinoRuntime::getInstance().getClock().get().readTime() * 1'000'000);
}

void delay(const int ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

void delayMicroseconds(const int us) {
    std::this_thread::sleep_for(std::chrono::microseconds(us));
}
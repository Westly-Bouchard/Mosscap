//
// Created by Westly Bouchard on 3/2/26.
//

#ifndef ARDUINORUNTIME_H
#define ARDUINORUNTIME_H

#include <unordered_map>
#include <cmath>

#define PI M_PI

#include <memory>

#include "Handle.hpp"
#include "Wire.h"
#include "../capability/ReadableDistance.h"
#include "../capability/ReadableEncoder.h"
#include "../capability/ReadableTime.h"
#include "../capability/WriteablePWM.h"

void setup();

void loop();

class ArduinoRuntime {
public:
    static ArduinoRuntime& getInstance();

    ArduinoRuntime(const ArduinoRuntime&) = delete;
    ArduinoRuntime& operator=(const ArduinoRuntime&) = delete;

    void bindPWM(int pin, WriteablePWM& pwm);
    void bindEncoder(int pin, ReadableEncoder& encoder);

    void bindTOF(ReadableDistance& tof);

    [[nodiscard]] Handle<WriteablePWM> getPWM(int pin) const;
    [[nodiscard]] Handle<ReadableEncoder> getEncoder(int pin) const;

    [[nodiscard]] Handle<ReadableDistance> getTOF() const;

    [[nodiscard]] Handle<ReadableTime> getClock() const;

private:
    ArduinoRuntime();
    ~ArduinoRuntime() = default;

    std::unordered_map<int, Handle<WriteablePWM>> pwmMap;
    std::unordered_map<int, Handle<ReadableEncoder>> encoderMap;

    std::unique_ptr<Handle<ReadableDistance>> i2cTOF;

    Handle<ReadableTime> clock;
};

/**
 * For simulated I2C peripherals
 * They just need a way to call `Wire.begin()` which, in this context,
 * doesn't actually do anything.
 */
inline SimWire Wire;

/**
 * Arduino time functions
 */

int millis();

int micros();

void delay(int ms);

void delayMicroseconds(int us);

template <typename T>
T constrain(T in, T low, T high) {
    if (in < low) { return low; }
    if (in > high) { return high; }
    return in;
}

#endif //ARDUINORUNTIME_H

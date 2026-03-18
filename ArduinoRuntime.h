//
// Created by Westly Bouchard on 3/2/26.
//

#ifndef ARDUINORUNTIME_H
#define ARDUINORUNTIME_H

#include <unordered_map>

#include "Handle.h"
#include "capability/ReadableEncoder.h"
#include "capability/ReadableTime.h"
#include "capability/WriteablePWM.h"

void setup();

void loop();

class ArduinoRuntime {
public:
    static ArduinoRuntime& getInstance();

    ArduinoRuntime(const ArduinoRuntime&) = delete;
    ArduinoRuntime& operator=(const ArduinoRuntime&) = delete;

    void bindPWM(int pin, WriteablePWM& pwm);
    void bindEncoder(int pin, ReadableEncoder& encoder);

    [[nodiscard]] Handle<WriteablePWM> getPWM(int pin) const;
    [[nodiscard]] Handle<ReadableEncoder> getEncoder(int pin) const;

    [[nodiscard]] Handle<ReadableTime> getClock() const;

private:
    ArduinoRuntime();
    ~ArduinoRuntime() = default;

    std::unordered_map<int, Handle<WriteablePWM>> pwmMap;
    std::unordered_map<int, Handle<ReadableEncoder>> encoderMap;

    Handle<ReadableTime> clock;
};

/**
 * Arduino time functions
 */

int millis();

int micros();

#endif //ARDUINORUNTIME_H

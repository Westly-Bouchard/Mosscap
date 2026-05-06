//
// Created by Westly Bouchard on 4/29/26.
//

#ifndef INC_441SIM_VL53L0X_H
#define INC_441SIM_VL53L0X_H

#include <cmath>

#include "ArduinoRuntime.h"

/**
 * A simulated time of flight sensor
 * This class implements shells for the most commonly used functions in this
 * library in accordance with the library source:
 * https://github.com/pololu/vl53l0x-arduino/blob/master/VL53L0X.h
 */
class VL53L0X {
public:
    static void startContinuous(int period_ms = 0) {}
    static void stopContinuous() {}

    static void setTimeout(int timeout) {}

    static bool init() { return true; }

    static int readRangeContinuousMillimeters() {
        return static_cast<int>(round(ArduinoRuntime::getInstance().getTOF().get().readDist() * 1000));
    }

    static int readRangeSingleMillimeters() {
        return readRangeContinuousMillimeters();
    }
};

#endif //INC_441SIM_VL53L0X_H

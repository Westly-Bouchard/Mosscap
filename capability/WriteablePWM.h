//
// Created by Westly Bouchard on 3/2/26.
//

#ifndef WRITEABLEPWM_H
#define WRITEABLEPWM_H

#include <atomic>

#include "Capability.h"

/**
 * Represents the ability of a piece of simulated hardware to be
 * written to as an analog value (pwm) between -255 and 255.
 */
struct WriteablePWM : Capability {
    [[nodiscard]] int get() const {
       return pwm.load();
    }

    void writePWM(const int value) {
        if (value <= 255 && value >= -255) {
            pwm.store(value);
        }
    }

protected:
    std::atomic_int pwm;

};

#endif //WRITEABLEPWM_H

//
// Created by Westly Bouchard on 3/2/26.
//

#ifndef WRITEABLEPWM_H
#define WRITEABLEPWM_H

#include "Capability.h"

/**
 * Represents the ability of a piece of simulated hardware to be
 * written to as an analog value (pwm) between -255 and 255.
 */
struct WriteablePWM : Capability {
    /**
     * Write a PWM value to the piece of hardware
     * @param value Signed integer between -255 and 255
     */
    virtual void writePWM(int value) = 0;
};

#endif //WRITEABLEPWM_H

//
// Created by Westly Bouchard on 3/2/26.
//

#include "SimMotor.h"

SimMotor::SimMotor(const MotorConfig& c) : config(c), pwm(0), velocity(0.0) {}

void SimMotor::writePWM(const int value) {
    pwm.store(value);
}

double SimMotor::getTorque(const double speed) {
    velocity = speed;
    const double backEmf = speed / config.kV;
    const double appliedVoltage = (pwm.load() / 255.0) * config.vBus;
    const double statorCurrent = (appliedVoltage - backEmf) / config.r;
    const double motorTorque = config.kT * statorCurrent;
    return motorTorque;
}

double SimMotor::getSpeed() const {
    return velocity;
}
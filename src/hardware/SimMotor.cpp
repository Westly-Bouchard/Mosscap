//
// Created by Westly Bouchard on 3/2/26.
//

#include "hardware/SimMotor.h"

#include <algorithm>
#include <cmath>

using namespace std;

SimMotor::SimMotor(const MotorConfig& c) : config(c), pwm(0), velocity(0.0) {}

void SimMotor::writePWM(const int value) {
    pwm.store(value);
}

double SimMotor::getTorque(const double speed) {
    velocity = speed;
    const double backEmf = speed / config.kV;
    const double appliedVoltage = (pwm.load() / 255.0) * config.vBus;
    const double statorCurrent = (appliedVoltage - backEmf) / config.r;
    const double electricalTorque = config.kT * statorCurrent;

    double netTorque = 0.0;

    if (abs(speed) < 1e-4) {
        // Stiction
        const double appliedStaticFriction = clamp(electricalTorque, -config.staticFriction, config.staticFriction);
        netTorque = electricalTorque - appliedStaticFriction;
    } else {
        // Kinetic
        // I'm gonna be honest this was suggested by an LLM
        // Use std::tanh to create a continuous curve so that the rk4 integrator doesn't freak out
        const double kineticFrictionTorque = config.kineticFriction * tanh(100.0 * speed);
        netTorque = electricalTorque - kineticFrictionTorque;
    }

    return netTorque;
}

double SimMotor::getSpeed() const {
    return velocity;
}
//
// Created by Westly Bouchard on 3/2/26.
//

#include "SimMotor.h"

SimMotor::SimMotor(const double kV, const double kT, const double R, const double vBus) : kV(kV), kT(kT), R(R),
    vBus(vBus),
    pwm(0) {
}

void SimMotor::writePWM(const int value) {
    pwm = value;
}

double SimMotor::getTorque(const double speed) const {
    const double backEmf = speed / kV;
    const double appliedVoltage = (pwm / 255.0) * vBus;
    const double statorCurrent = (appliedVoltage - backEmf) / R;
    const double motorTorque = kT * statorCurrent;
    return motorTorque;
}

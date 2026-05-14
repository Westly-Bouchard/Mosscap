//
// Created by west on 5/13/26.
//

#include "plant/TankPlant.h"

TankPlant::TankPlant(const WMRConfig& config) : config(config) {}

void TankPlant::operator()(const state_t &x, state_t &dxdt, double t) const {
    // Calculate forces in the body frame
    // Force in the y direction is always zero (in this simplified case)
    const double Fl = inputs.at(0) / config.wheelRadius;
    const double Fr = inputs.at(1) / config.wheelRadius;

    const double bFx = Fl + Fr;
    const double bFy = 0;
    const double bTz = (config.trackWidth / 2.0) * (Fr - Fl);

    // Transform forces into the world frame
    const double s = sin(x.at(2));
    const double c = cos(x.at(2));

    const double wFx = bFx * c - bFy * s;
    const double wFy = bFx * s + bFy * c;
    const double wTz = bTz;

    // Update the derivative vector
    dxdt.at(0) = x.at(3);
    dxdt.at(1) = x.at(4);
    dxdt.at(2) = x.at(5);

    // Update acceleration components of the derivative vector
    dxdt.at(3) = (1.0 / config.mass) * wFx;
    dxdt.at(4) = (1.0 / config.mass) * wFy;
    dxdt.at(5) = (1.0 / config.inertia) * wTz;
}

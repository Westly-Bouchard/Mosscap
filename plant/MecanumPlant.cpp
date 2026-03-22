//
// Created by west on 3/21/26.
//

#include "MecanumPlant.h"

#include <cmath>

MecanumPlant::MecanumPlant(const double m, const double tW, const double wB, const double wR) : wheelRadius(wR),
	wheelBase(wB), trackWidth(tW), m_e(m), I_e((1.0 / 12.0) * m * (pow(tW, 2) + pow(wB, 2))) {}

void MecanumPlant::setTorques(const double FL, const double FR, const double BL, const double BR) {
    torques.at(0) = FL;
    torques.at(1) = FR;
    torques.at(2) = BL;
    torques.at(3) = BR;
}

void MecanumPlant::operator()(const state_t &x, state_t &dxdt, double t) const {
    // Calculate forces in the body frame
    const double bFy = (torques.at(0) + torques.at(1) + torques.at(2) +
                        torques.at(3)) / wheelRadius;

    const double bFx = (-torques.at(0) + torques.at(1) + torques.at(2) -
                        torques.at(3)) / wheelRadius;

    const double bTz = -(wheelBase / 2 + trackWidth / 2) * (torques.at(0) -
                        torques.at(1) + torques.at(2) - torques.at(3)) /
                        wheelRadius;

    // Transform forces into the world frame
    const double s = sin(x.at(2));
    const double c = cos(x.at(2));

    const double wFy = bFy * c + bFx * s;
    const double wFx = bFx * c - bFy * s;
    const double wTz = bTz;

    // Update the derivative vector
    dxdt.at(0) = x.at(3);
    dxdt.at(1) = x.at(4);
    dxdt.at(2) = x.at(5);

    dxdt.at(3) = (1.0 / m_e) * wFx;
    dxdt.at(4) = (1.0 / m_e) * wFy;
    dxdt.at(5) = (1.0 / I_e) * wTz;
}

//
// Created by west on 3/21/26.
//

#include "MecanumPlant.h"

#include <cmath>

using namespace std;

MecanumPlant::MecanumPlant(const double m, const double tW, const double wB, const double wR) : wheelRadius(wR),
	wheelBase(wB), trackWidth(tW), m_e(m), I_e((1.0 / 12.0) * m * (pow(tW, 2) + pow(wB, 2))) {}

void MecanumPlant::operator()(const state_t &x, state_t &dxdt, double t) const {
    // Calculate forces in the body frame
    const double bFy = (inputs.at(0) + inputs.at(1) + inputs.at(2) +
                        inputs.at(3)) / wheelRadius;

    const double bFx = (-inputs.at(0) + inputs.at(1) + inputs.at(2) -
                        inputs.at(3)) / wheelRadius;

    const double bTz = -(wheelBase / 2 + trackWidth / 2) * (inputs.at(0) -
                        inputs.at(1) + inputs.at(2) - inputs.at(3)) /
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

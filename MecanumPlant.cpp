//
// Created by Westly Bouchard on 3/2/26.
//

#include "MecanumPlant.h"

using namespace std;

MecanumPlant::MecanumPlant(const double chassisMass, const double trackWidth, const double wheelBase,
                           const double wheelMass, const double wheelRadius) : chassisMass(chassisMass),
                                                                               trackWidth(trackWidth),
                                                                               wheelBase(wheelBase),
                                                                               wheelMass(wheelMass),
                                                                               wheelRadius(wheelRadius) {
    I_k = 0.5 * wheelMass * pow(wheelRadius, 2);
    I_pc = (1.0 / 12.0) * chassisMass * (pow(trackWidth, 2) + pow(wheelBase, 2));
    m_e = chassisMass + 4 * I_k / pow(wheelRadius, 2);
    I_e = I_pc + 4 * I_k * (pow(wheelBase / 2 + trackWidth / 2, 2), pow(wheelRadius, 2));

    tau_1 = 0;
    tau_2 = 0;
    tau_3 = 0;
    tau_4 = 0;
}

void MecanumPlant::setTorques(const double FL, const double FR, const double BL, const double BR) {
    tau_1 = FL;
    tau_2 = FR;
    tau_3 = BL;
    tau_4 = BR;
}

void MecanumPlant::operator()(const array<double, 6> &x, array<double, 6> &dxdt, const double t) const {

    const double bFy = (tau_1 + tau_2 + tau_3 + tau_4) / wheelRadius;
    const double bFx = (-tau_1 + tau_2 + tau_3 - tau_4) / wheelRadius;
    const double bTz = -(wheelBase / 2 + trackWidth / 2) * (tau_1 - tau_2 + tau_3 - tau_4) / wheelRadius;

    const double s = sin(x.at(2));
    const double c = cos(x.at(2));

    const double wFy = bFy * c + bFx * s;
    const double wFx = bFx * c - bFy * s;
    const double wTz = bTz;

    dxdt.at(0) = x.at(3);
    dxdt.at(1) = x.at(4);
    dxdt.at(2) = x.at(5);

    dxdt.at(3) = (1.0 / m_e) * wFx;
    dxdt.at(4) = (1.0 / m_e) * wFy;
    dxdt.at(5) = (1.0 / I_e) * wTz;
}

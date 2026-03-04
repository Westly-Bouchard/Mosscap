//
// Created by Westly Bouchard on 3/2/26.
//

#ifndef MECANUMPLANT_H
#define MECANUMPLANT_H
#include <array>


class MecanumPlant {
public:
    MecanumPlant(double chassisMass, double trackWidth, double wheelBase, double wheelMass, double wheelRadius);

    void operator() (const std::array<double, 6> &x, std::array<double, 6> &dxdt, double t) const;

    void setTorques(double FL, double FR, double BL, double BR);

private:
    double chassisMass, trackWidth, wheelBase, wheelMass, wheelRadius;

    double m_e, I_e, I_k, I_pc;

    double tau_1, tau_2, tau_3, tau_4;
};



#endif //MECANUMPLANT_H

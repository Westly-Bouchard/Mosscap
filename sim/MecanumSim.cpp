//
// Created by west on 3/22/26.
//

#include "MecanumSim.h"

#include <ranges>

#include "../plant/MecanumPlant.h"

using namespace std;

MecanumSim::MecanumSim(const double m, const double tW, const double wB, const double wR)
                        :  wheelRadius(wR), wheelBase(wB), trackWidth(tW) {
    this->plant = std::make_unique<MecanumPlant>(m, tW, wB, wR);
}

void MecanumSim::updateHardware() {
    for (const auto vels = fwdKinematics(); auto&& [enc, vel] : views::zip(views::as_const(encoders), vels)) {
        enc->updatePosition(vel * dt);
    }
}

void MecanumSim::setPlantInputs() {
    MecanumPlant::input_t torques;
    // Wow I love modern C++ so much, this is so intelligible
    for (const auto vels = fwdKinematics(); auto &&[motor, vel, torque] : std::views::zip(
             views::as_const(motors), vels, torques)) {
        torque = motor->getTorque(vel);
    }

    plant->setInputs(torques);
}

std::array<double, 4> MecanumSim::fwdKinematics() const {
    // Body frame velocities for forward kinematics
    const double c = cos(state.at(2));
    const double s = sin(state.at(2));

    const double bVy = state.at(4) * c - state.at(3) * s;
    const double bVx = state.at(3) * c + state.at(4) * s;

    const double oOR = 1 / wheelRadius;
    const double mid = wheelBase / 2 + trackWidth / 2;
    const double w = state.at(5);

    return {
        oOR * (bVy - bVx - mid * w),
        oOR * (bVy + bVx + mid * w),
        oOR * (bVy + bVx - mid * w),
        oOR * (bVy - bVx + mid * w)
    };
}

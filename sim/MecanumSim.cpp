//
// Created by west on 3/22/26.
//

#include "MecanumSim.h"

#include <ranges>

#include "../plant/MecanumPlant.h"

using namespace std;

MecanumSim::MecanumSim(const MecanumConfig &config)
                        :  config(config) {
    this->plant = std::make_unique<MecanumPlant>(config);
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

    const double oOR = 1 / config.wheelRadius;
    const double mid = config.wheelBase / 2 + config.trackWidth / 2;
    const double w = state.at(5);

    return {
        oOR * (bVy - bVx - mid * w),
        oOR * (bVy + bVx + mid * w),
        oOR * (bVy + bVx - mid * w),
        oOR * (bVy - bVx + mid * w)
    };
}

SimMotor& MecanumSim::registerMotor(const unsigned int idx, std::unique_ptr<SimMotor> motor) {
    motors.at(idx) = std::move(motor);
    return *motors.at(idx);
}

SimEncoder& MecanumSim::registerEncoder(const unsigned int idx, std::unique_ptr<SimEncoder> encoder) {
    encoders.at(idx) = std::move(encoder);
    return *encoders.at(idx);
}
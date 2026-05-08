//
// Created by west on 3/22/26.
//

#include "sim/MecanumSim.h"

#include <cmath>
#include <ranges>
#include <sstream>

#include "../plant/MecanumPlant.h"

using namespace std;

MecanumSim::MecanumSim(const MecanumConfig &config)
                        :  config(config) {
    // Not sure why I can't just pass the config param
    // Mecanum plant ctor also passes by const ref and copies
    // So who knows why this doesn't work
    this->plant = std::make_unique<MecanumPlant>(this->config);
}

void MecanumSim::setPose(const double x, const double y, const double theta) {
    state.at(0) = x;
    state.at(1) = y;
    state.at(2) = theta * (M_PI / 180.0);
}

void MecanumSim::updateHardware() {
    // Update distance traveled by each wheel
    for (const auto vels = fwdKinematics(); auto&& [enc, vel] : views::zip((encoders), vels)) {
        enc->updatePosition(vel * dt);
    }

    // Update tof position
    tof->update(state.at(0), state.at(1), state.at(2));
}

void MecanumSim::setPlantInputs() {
    MecanumPlant::input_t torques;
    // Wow I love modern C++ so much, this is so intelligible
    for (const auto vels = fwdKinematics(); auto &&[motor, vel, torque] : views::zip(
             motors, vels, torques)) {
        torque = motor->getTorque(vel);
    }

    plant->setInputs(torques);
}

std::array<double, 4> MecanumSim::fwdKinematics() const {
    // Body frame velocities for forward kinematics
    const double c = cos(state.at(2));
    const double s = sin(state.at(2));

    const double bVx = state.at(4) * c - state.at(3) * s;
    const double bVy = state.at(3) * c + state.at(4) * s;

    const double oOverR = 1 / config.wheelRadius;
    const double mid = config.wheelBase / 2 + config.trackWidth / 2;
    const double w = state.at(5);

    return {
        oOverR * (bVy - bVx - mid * w),
        oOverR * (bVy + bVx + mid * w),
        oOverR * (bVy + bVx - mid * w),
        oOverR * (bVy - bVx + mid * w)
    };
}

void MecanumSim::registerMotor(const unsigned int idx, MotorConfig c, const int pin) {
    motors.at(idx) = std::make_unique<SimMotor>(c);
    ArduinoRuntime::getInstance().bindPWM(pin, *motors.at(idx));
}

void MecanumSim::registerEncoder(const unsigned int idx, const int cpr, const int pin) {
    encoders.at(idx) = std::make_unique<SimEncoder>(cpr);
    ArduinoRuntime::getInstance().bindEncoder(pin, *encoders.at(idx));
}

void MecanumSim::registerTOF(TOFConfig c) {
    tof = std::make_unique<SimTOF>(c);
    ArduinoRuntime::getInstance().bindTOF(*tof);
}

void MecanumSim::draw() const {
    // Draw robot
    Renderer::drawRect(state.at(0), state.at(1), state.at(2), 0.200, 0.245, {255, 255, 255});
}

void MecanumSim::write() const {
    stringstream ss;

    if (Telemetry::section("Robot Position")) {
        ss << "X\t: " << state.at(0);
        Telemetry::text(ss.str());

        ss.str("");
        ss << "Y\t: " << state.at(1);
        Telemetry::text(ss.str());

        double t = state.at(2);

        while (t > 2 * M_PI) t -= 2 * M_PI;

        ss.str("");
        ss << "Theta\t: " << t * 180.0 / M_PI;
        Telemetry::text(ss.str());
    }

    ss.str("");

    if (Telemetry::section("Encoders")) {
        for (auto&& [e, label] : std::views::zip(encoders, std::array{"FL", "FR", "BL", "BR"})) {
            ss << label << " counts: " << e->readCount();
            Telemetry::text(ss.str());
            ss.str("");
        }
    }

    ss.str("");

    if (Telemetry::section("TOF")) {
        ss << "Current reading: " << tof->getDist();
        Telemetry::text(ss.str());
    }
}
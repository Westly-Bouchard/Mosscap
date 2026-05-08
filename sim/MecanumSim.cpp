//
// Created by west on 3/22/26.
//

#include "MecanumSim.h"

#include <cmath>
#include <ranges>

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

SimMotor& MecanumSim::registerMotor(const unsigned int idx, std::unique_ptr<SimMotor> motor) {
    motors.at(idx) = std::move(motor);
    return *motors.at(idx);
}

SimEncoder& MecanumSim::registerEncoder(const unsigned int idx, std::unique_ptr<SimEncoder> encoder) {
    encoders.at(idx) = std::move(encoder);
    return *encoders.at(idx);
}

SimTOF& MecanumSim::registerTOF(std::unique_ptr<SimTOF> sensor) {
    tof = std::move(sensor);
    return *tof;
}

void MecanumSim::draw() const {
    // Draw robot itself
    Renderer::drawRect(state.at(0), state.at(1), state.at(2), 0.200, 0.245, {255, 255, 255});

    // beginTelemetryWindow();
    // Write basic telemetry
    // if (ImGui::CollapsingHeader("Robot Position", ImGuiTreeNodeFlags_DefaultOpen)) {
    //     ImGui::Text("X\t: %4.2f", state.at(0));
    //     ImGui::Text("Y\t: %4.2f", state.at(1));
    //     ImGui::Text("theta: %4.2f", state.at(2) * 180.0 / M_PI);
    // }
    //
    // if (ImGui::CollapsingHeader("Encoders", ImGuiTreeNodeFlags_DefaultOpen)) {
    //     for (auto&& [e, label] : std::views::zip(encoders, std::array{"FL", "FR", "BL", "BR"})) {
    //         ImGui::Text("%s counts: %i", label, e->readCount());
    //     }
    // }
    //
    // if (ImGui::CollapsingHeader("TOF", ImGuiTreeNodeFlags_DefaultOpen)) {
    //     ImGui::Text("X\t: %4.2f", tof->getDist());
    // }
    //
    // const ImGuiIO& io = ImGui::GetIO(); (void)io;
    //
    // ImGui::Separator();
    // ImGui::Text("Average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
    //
    // endTelemetryWindow();
}
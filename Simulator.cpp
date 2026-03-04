//
// Created by Westly Bouchard on 3/2/26.
//

#include "Simulator.h"

#include "ArduinoRuntime.h"

using namespace std;

Simulator::Simulator(
    const double chassisMass,
    const double trackWidth,
    const double wheelBase,
    const double wheelMass,
    const double wheelRadius
    ) :
plant(chassisMass, trackWidth, wheelBase, wheelMass, wheelRadius),
trackWidth(trackWidth),
wheelBase(wheelBase),
wheelRadius(wheelRadius),
simTime(0.0) {
    state = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};

    // Call into Arduino setup function
    setup();
}

void Simulator::setPose(const Pose &p) {
    state.at(0) = p.x;
    state.at(1) = p.y;
    state.at(2) = p.theta;
}

Pose Simulator::getPose() const {
    return {state.at(0), state.at(1), state.at(2)};
}

void Simulator::update(double &acc) {
    while (acc > dt) {
        // Body frame velocities for forward kinematics
        const double c = cos(state.at(2));
        const double s = sin(state.at(2));

        const double bVy = state.at(4) * c - state.at(3) * s;
        const double bVx = state.at(3) * c + state.at(4) * s;

        // Call the Arduino update function
        loop();

        // Compute velocities of motors (this is needed for torque calculations)
        const double omega_1 = (1 / wheelRadius) * (bVy - bVx - (wheelBase / 2 + trackWidth / 2) * state.at(5));
        const double omega_2 = (1 / wheelRadius) * (bVy + bVx + (wheelBase / 2 + trackWidth / 2) * state.at(5));
        const double omega_3 = (1 / wheelRadius) * (bVy + bVx - (wheelBase / 2 + trackWidth / 2) * state.at(5));
        const double omega_4 = (1 / wheelRadius) * (bVy - bVx + (wheelBase / 2 + trackWidth / 2) * state.at(5));

        plant.setTorques(
            FL->getTorque(omega_1),
            FR->getTorque(omega_2),
            BL->getTorque(omega_3),
            BR->getTorque(omega_4)
        );

        stepper.do_step(plant, state, simTime, dt);
        simTime += dt;
        acc -= dt;
    }
}


SimMotor& Simulator::registerFLMotor(unique_ptr<SimMotor> m) {
    FL = std::move(m);
    return *FL;
}

SimMotor& Simulator::registerFRMotor(unique_ptr<SimMotor> m) {
    FR = std::move(m);
    return *FR;
}

SimMotor& Simulator::registerBLMotor(unique_ptr<SimMotor> m) {
    BL = std::move(m);
    return *BL;
}

SimMotor& Simulator::registerBRMotor(unique_ptr<SimMotor> m) {
    BR = std::move(m);
    return *BR;
}

SimEncoder& Simulator::registerFLEncoder(unique_ptr<SimEncoder> e) {
    FL_encoder = std::move(e);
    return *FL_encoder;
}

SimEncoder& Simulator::registerFREncoder(unique_ptr<SimEncoder> e) {
    FR_encoder = std::move(e);
    return *FR_encoder;
}

SimEncoder& Simulator::registerBLEncoder(unique_ptr<SimEncoder> e) {
    BL_encoder = std::move(e);
    return *BL_encoder;
}

SimEncoder& Simulator::registerBREncoder(unique_ptr<SimEncoder> e) {
    BR_encoder = std::move(e);
    return *BR_encoder;
}

//
// Created by west on 5/13/26.
//

#include "sim/TankSim.h"

#include "plant/TankPlant.h"

using namespace std;

TankSim::TankSim(const WMRConfig &config) : config(config) {
    this->plant = std::make_unique<TankPlant>(this->config);
}

void TankSim::setPose(const double x, const double y, const double theta) {
    state.at(0) = x;
    state.at(1) = y;
    state.at(2) = theta * (M_PI / 180.0);
}

void TankSim::updateHardware() {
    const auto [wL, wR] = fwdKinematics();

    leftEncoder->updatePosition(wL *dt);
    rightEncoder->updatePosition(wR *dt);

    tof->update(state.at(0), state.at(1), state.at(2));
}

void TankSim::setPlantInputs() {
    const auto [wL, wR] = fwdKinematics();
    TankPlant::input_t torques;
    torques.at(0) = leftMotor->getTorque(wL);
    torques.at(1) = rightMotor->getTorque(wR);

    plant->setInputs(torques);
}

std::array<double, 2> TankSim::fwdKinematics() const {
    // Use current state to compute the velocities of the left and right wheels
    const double wVx = state.at(3);
    const double wVy = state.at(4);
    const double wWz = state.at(5);

    const double s = sin(state.at(2));
    const double c = cos(state.at(2));

    const double bVx = wVx * c + wVy * s;

    // Should be zero
    // const double bVy = wVy * c - wVx * s;

    const double wL = (bVx - (wWz * config.trackWidth / 2.0)) / config.wheelRadius;
    const double wR = (bVx + (wWz * config.trackWidth / 2.0)) / config.wheelRadius;

    return {wL, wR};
}

std::array<double, 2> TankSim::bodyToWorld(const double x, const double y) const {
    const double s = sin(state.at(2));
    const double c = cos(state.at(2));

    const double wX = x * c - y * s + state.at(0);
    const double wY = x * s + y * c + state.at(1);

    return {wX, wY};
}

void TankSim::registerLeftMotor(MotorConfig c, const int pin) {
    leftMotor = std::make_unique<SimMotor>(c);
    ArduinoRuntime::getInstance().bindPWM(pin, *leftMotor);
}

void TankSim::registerRightMotor(MotorConfig c, const int pin) {
    rightMotor = std::make_unique<SimMotor>(c);
    ArduinoRuntime::getInstance().bindPWM(pin, *rightMotor);
}

void TankSim::registerLeftEncoder(int cpr, const int pin) {
    leftEncoder = std::make_unique<SimEncoder>(cpr);
    ArduinoRuntime::getInstance().bindEncoder(pin, *leftEncoder);
}

void TankSim::registerRightEncoder(int cpr, const int pin) {
    rightEncoder = std::make_unique<SimEncoder>(cpr);
    ArduinoRuntime::getInstance().bindEncoder(pin, *rightEncoder);
}

void TankSim::registerTOF(TOFConfig c) {
    tof = std::make_unique<SimTOF>(c);
    ArduinoRuntime::getInstance().bindTOF(*tof);
}

void TankSim::draw() const {
    // Here I assume that the wheels are 3cm wide
    const double chassisWidth = config.trackWidth - 0.03;

    // Draw chassis
    Renderer::drawRect(
        state.at(0),
        state.at(1),
        state.at(2),
        config.wheelBase,
        chassisWidth,
        {0xf5, 0xf5, 0xf5}
    );

    // Mark the forward direction with an arrow
    const double arrowLength = config.wheelBase * 0.75;
    const double arrowThickness = config.trackWidth * 0.1;
    const double arrowHeadLength = 2.2 * sqrt(pow(arrowLength / 4.0, 2) / 2.0);

    Renderer::drawRect(
        state.at(0),
        state.at(1),
        state.at(2),
        arrowLength,
        arrowThickness,
        {0, 255, 0}
    );

    const auto [headLX, headLY] = bodyToWorld(arrowLength * (3.0/8.0), arrowLength * (1.0/8.0));
    Renderer::drawRect(
        headLX,
        headLY,
        state.at(2) - M_PI / 4.0,
        arrowHeadLength,
        arrowThickness,
        {0, 255, 0}
    );

    const auto [headRX, headRY] = bodyToWorld(arrowLength * (3.0/8.0), -arrowLength * (1.0/8.0));
    Renderer::drawRect(
        headRX,
        headRY,
        state.at(2) + M_PI / 4.0,
        arrowHeadLength,
        arrowThickness,
        {0, 255, 0}
    );

    // Draw wheels
    const auto [wheelLX, wheelLY] = bodyToWorld(0, chassisWidth / 2.0 + 0.015);
    Renderer::drawRect(
        wheelLX,
        wheelLY,
        state.at(2),
        config.wheelRadius * 2.0,
        0.03,
        {0xb4, 0xb4, 0xb4}
    );

    const auto [wheelRX, wheelRY] = bodyToWorld(0, -chassisWidth / 2.0 - 0.015);
    Renderer::drawRect(
        wheelRX,
        wheelRY,
        state.at(2),
        config.wheelRadius * 2.0,
        0.03,
        {0xb4, 0xb4, 0xb4}
    );
}

void TankSim::write() const {
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
        ss << "Left counts: " << leftEncoder->readCount();
        Telemetry::text(ss.str());
        ss.str("");
        ss << "Right counts: " << rightEncoder->readCount();
        Telemetry::text(ss.str());
        ss.str("");
    }

    ss.str("");

    if (Telemetry::section("TOF")) {
        ss << "Current reading: " << tof->getDist();
        Telemetry::text(ss.str());
    }
}
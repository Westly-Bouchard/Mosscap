//
// Created by Westly Bouchard on 3/2/26.
//

#ifndef SIM_H
#define SIM_H

#include "../sim/MecanumSim.h"
#include "../sim/SimulatorBase.h"
#include "../arduino/ArduinoRuntime.h"

#include "../hardware/SimEncoder.h"
#include "../hardware/SimMotor.h"

#include "../graphics/Renderer.h"

#include "Constants.h"

void drawPath();

/**
 * Initialize the simulator
 * This is where a user would set up a specific simulator
 * for their code.
 * @return Pointer to sim
 */
inline std::unique_ptr<SimulatorBase> simInit() {
    // Set rendering scale
    Renderer::setScale(3.0);

    // Tells the renderer to draw the course (green lines) below the robot
    Renderer::getInstance().registerDrawFunction(-2, drawPath);

    // Configuration of the robot
    MecanumConfig config{Constants::CHASSIS_MASS, Constants::TRACKWIDTH, Constants::WHEELBASE, Constants::WHEEL_RADIUS};

    TOFConfig tofConfig{0, 0, 0};
    tofConfig.boundingBox = BoundingBox(3, 3);

    // Add an obstacle for the sensor to detect
    tofConfig.obstacles.push_back(std::make_shared<Box>(1.0, 0.25, Vec{1.5, 2.0}, 0));

    // Create robot
    auto robot = std::make_unique<MecanumSim>(config);
    // robot->setPose(2.5, 1.2, 90);
    robot->setPose(1.5, 1.5, 0);

    auto& tof = robot->registerTOF(std::make_unique<SimTOF>(tofConfig));
    ArduinoRuntime::getInstance().bindTOF(tof);

    // Configuration of motors
    // In theory you could have a different config for each motor if you wanted
    // But in this case each motor is exactly the same
    MotorConfig mConfig{Constants::Motor::K_V, Constants::Motor::K_T, Constants::Motor::R, Constants::V_BUS};

    // Create motors and register them with the ArduinoRuntime
    auto& FL = robot->registerMotor(MecanumSim::FL, std::make_unique<SimMotor>(MotorConfig(
        Constants::Motor::K_V, Constants::Motor::K_T, Constants::Motor::R, Constants::V_BUS, 0.239
    )));
    ArduinoRuntime::getInstance().bindPWM(28, FL);

    auto& FR = robot->registerMotor(MecanumSim::FR, std::make_unique<SimMotor>(MotorConfig(
        Constants::Motor::K_V, Constants::Motor::K_T, Constants::Motor::R, Constants::V_BUS, 0.239
    )));
    ArduinoRuntime::getInstance().bindPWM(24, FR);

    auto& BL = robot->registerMotor(MecanumSim::BL, std::make_unique<SimMotor>(MotorConfig(
        Constants::Motor::K_V, Constants::Motor::K_T, Constants::Motor::R, Constants::V_BUS, 0.239
    )));
    ArduinoRuntime::getInstance().bindPWM(4, BL);

    auto& BR = robot->registerMotor(MecanumSim::BR, std::make_unique<SimMotor>(MotorConfig(
        Constants::Motor::K_V, Constants::Motor::K_T, Constants::Motor::R, Constants::V_BUS, 0.239
    )));
    ArduinoRuntime::getInstance().bindPWM(7, BR);

    // Create encoders and register them with the ArduinoRuntime
    SimEncoder& FL_encoder = robot->registerEncoder(MecanumSim::FL, std::make_unique<SimEncoder>(1500));
    ArduinoRuntime::getInstance().bindEncoder(3, FL_encoder);

    SimEncoder& FR_encoder = robot->registerEncoder(MecanumSim::FR, std::make_unique<SimEncoder>(1500));
    ArduinoRuntime::getInstance().bindEncoder(2, FR_encoder);

    SimEncoder& BL_encoder = robot->registerEncoder(MecanumSim::BL, std::make_unique<SimEncoder>(1500));
    ArduinoRuntime::getInstance().bindEncoder(19, BL_encoder);

    SimEncoder& BR_encoder = robot->registerEncoder(MecanumSim::BR, std::make_unique<SimEncoder>(1500));
    ArduinoRuntime::getInstance().bindEncoder(18, BR_encoder);

    return robot;
}

void drawPath() {
    static constexpr std::array<std::pair<double, double>, 8> pathPoints = {{
        {1.0, 0.3},
        {-0.4, 0.3},
        {-0.4, 1.2},
        {-1.0, 1.2},
        {-1.0, -0.6},
        {0.0, -0.6},
        {0.0, -1.2},
        {1.0, -1.2}
    }};

    std::array<ImVec2, pathPoints.size() + 1> vertices;
    for (auto&& [p, v] : std::views::zip(pathPoints, vertices)) {
        v.x = p.first * pxPerMeter + 400;
        v.y = p.second * pxPerMeter + 400;
    }

    // This is kind of hacky but whatever
    vertices.at(8) = vertices.at(0);
    vertices.at(8).y += 3.1;

    ImGui::GetWindowDrawList()->AddPolyline(vertices.data(), vertices.size(), 0xFF00FF00, 0, 7.0);
}

#endif //SIM_H

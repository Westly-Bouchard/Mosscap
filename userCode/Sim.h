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

#include "../util/Renderer.h"

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

    // Create robot
    auto robot = std::make_unique<MecanumSim>(config);
    // robot->setPose(2.5, 1.2, 90);
    robot->setPose(1.5, 1.5, 0);

    TOFConfig tofConfig{0, 0, 0};
    tofConfig.boundingBox = BoundingBox(3, 3);

    // Add an obstacle for the sensor to detect
    tofConfig.obstacles.push_back(std::make_shared<Box>(1.0, 0.25, Vec{1.5, 2.0}, 0));

    robot->registerTOF(tofConfig);

    // Configuration of motors
    // In theory you could have a different config for each motor if you wanted
    // But in this case each motor is exactly the same
    const MotorConfig mConfig{Constants::Motor::K_V, Constants::Motor::K_T, Constants::Motor::R, Constants::V_BUS};

    robot->registerMotor(MecanumSim::FL, mConfig, 28);
    robot->registerMotor(MecanumSim::FR, mConfig, 24);
    robot->registerMotor(MecanumSim::BL, mConfig, 4);
    robot->registerMotor(MecanumSim::BR, mConfig, 7);

    robot->registerEncoder(MecanumSim::FL, 1500, 3);
    robot->registerEncoder(MecanumSim::FR, 1500, 2);
    robot->registerEncoder(MecanumSim::BL, 1500, 19);
    robot->registerEncoder(MecanumSim::BR, 1500, 18);

    ArduinoRuntime::getInstance().createButton("Test", 1);

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

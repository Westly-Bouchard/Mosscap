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

#include "Constants.h"

/**
 * Initialize the simulator
 * This is where a user would set up a specific simulator
 * for their code.
 * @return Pointer to sim
 */
inline std::unique_ptr<SimulatorBase> simInit() {
    // Configuration of the robot
    MecanumConfig config{Constants::CHASSIS_MASS, Constants::TRACKWIDTH, Constants::WHEELBASE, Constants::WHEEL_RADIUS};

    TOFConfig tofConfig{0, 0, 0};
    tofConfig.boundingBox = BoundingBox(3, 3);

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

#endif //SIM_H

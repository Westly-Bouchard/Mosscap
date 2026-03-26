//
// Created by Westly Bouchard on 3/2/26.
//

#ifndef SIM_H
#define SIM_H

#include "../sim/MecanumSim.h"
#include "../sim/SimulatorBase.h"
#include "../ArduinoRuntime.h"

#include "../hardware/SimEncoder.h"
#include "../hardware/SimMotor.h"

#include "Constants.h"

inline std::unique_ptr<SimulatorBase> simInit() {
    // Configuration of the robot
    MecanumConfig config{Constants::CHASSIS_MASS, Constants::TRACKWIDTH, Constants::WHEELBASE, Constants::WHEEL_RADIUS};

    // Create robot
    auto robot = std::make_unique<MecanumSim>(config);

    // Configuration of motors
    // In theory you could have a different config for each motor if you wanted
    // But in this case each motor is exactly the same
    MotorConfig mConfig{Constants::Motor::K_V, Constants::Motor::K_T, Constants::Motor::R, Constants::V_BUS};

    // Create motors and register them with the ArduinoRuntime
    auto& FL = robot->registerMotor(MecanumSim::FL, std::make_unique<SimMotor>(mConfig));
    ArduinoRuntime::getInstance().bindPWM(9, FL);

    auto& FR = robot->registerMotor(MecanumSim::FR, std::make_unique<SimMotor>(mConfig));
    ArduinoRuntime::getInstance().bindPWM(10, FR);

    auto& BL = robot->registerMotor(MecanumSim::BL, std::make_unique<SimMotor>(mConfig));
    ArduinoRuntime::getInstance().bindPWM(11, BL);

    auto& BR = robot->registerMotor(MecanumSim::BR, std::make_unique<SimMotor>(mConfig));
    ArduinoRuntime::getInstance().bindPWM(12, BR);

    // Create encoders and register them with the ArduinoRuntime
    SimEncoder& FL_encoder = robot->registerEncoder(MecanumSim::FL, std::make_unique<SimEncoder>(1500));
    ArduinoRuntime::getInstance().bindEncoder(20, FL_encoder);

    SimEncoder& FR_encoder = robot->registerEncoder(MecanumSim::FR, std::make_unique<SimEncoder>(1500));
    ArduinoRuntime::getInstance().bindEncoder(21, FR_encoder);

    SimEncoder& BL_encoder = robot->registerEncoder(MecanumSim::BL, std::make_unique<SimEncoder>(1500));
    ArduinoRuntime::getInstance().bindEncoder(22, BL_encoder);

    SimEncoder& BR_encoder = robot->registerEncoder(MecanumSim::BR, std::make_unique<SimEncoder>(1500));
    ArduinoRuntime::getInstance().bindEncoder(23, BR_encoder);

    return robot;
}

#endif //SIM_H

//
// Created by Westly Bouchard on 3/2/26.
//

#ifndef SIM_H
#define SIM_H

#include "../Simulator.h"
#include "../ArduinoRuntime.h"

#include "../hardware/SimEncoder.h"
#include "../hardware/SimMotor.h"

#include "Constants.h"

inline std::unique_ptr<Simulator> simInit() {
    auto robot = std::make_unique<Simulator>(
        Constants::CHASSIS_MASS,
        Constants::TRACKWIDTH,
        Constants::WHEELBASE,
        Constants::WHEEL_MASS,
        Constants::WHEEL_RADIUS
    );

    robot->setPose({1.5, 1.5, 0});

    auto FL_ptr = std::make_unique<SimMotor>(
        Constants::Motor::K_V,
        Constants::Motor::K_T,
        Constants::Motor::R,
        Constants::V_BUS
    );

    // Create a motor
    SimMotor& FL = robot->registerFLMotor(std::move(FL_ptr));

    // Wire it to the arduino runtime
    ArduinoRuntime::getInstance().bindPWM(9, FL);

    // Repeat for other hardware
    SimMotor& FR = robot->registerFRMotor(std::make_unique<SimMotor>(
        Constants::Motor::K_V,
        Constants::Motor::K_T,
        Constants::Motor::R,
        Constants::V_BUS
    ));

    // Wire it to the arduino runtime
    ArduinoRuntime::getInstance().bindPWM(10, FR);

    SimMotor& BL = robot->registerBLMotor(std::make_unique<SimMotor>(
        Constants::Motor::K_V,
        Constants::Motor::K_T,
        Constants::Motor::R,
        Constants::V_BUS
    ));

    // Wire it to the arduino runtime
    ArduinoRuntime::getInstance().bindPWM(11, BL);

    SimMotor& BR = robot->registerBRMotor(std::make_unique<SimMotor>(
        Constants::Motor::K_V,
        Constants::Motor::K_T,
        Constants::Motor::R,
        Constants::V_BUS
    ));

    // Wire it to the arduino runtime
    ArduinoRuntime::getInstance().bindPWM(12, BR);

    SimEncoder& FL_encoder = robot->registerFLEncoder(std::make_unique<SimEncoder>(1500));
    ArduinoRuntime::getInstance().bindEncoder(20, FL_encoder);

    SimEncoder& FR_encoder = robot->registerFREncoder(std::make_unique<SimEncoder>(1500));
    ArduinoRuntime::getInstance().bindEncoder(21, FR_encoder);

    SimEncoder& BL_encoder = robot->registerBLEncoder(std::make_unique<SimEncoder>(1500));
    ArduinoRuntime::getInstance().bindEncoder(22, BL_encoder);

    SimEncoder& BR_encoder = robot->registerBREncoder(std::make_unique<SimEncoder>(1500));
    ArduinoRuntime::getInstance().bindEncoder(23, BR_encoder);

    return robot;
}

#endif //SIM_H

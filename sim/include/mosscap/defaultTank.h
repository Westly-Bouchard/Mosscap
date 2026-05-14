//
// Created by west on 5/13/26.
//

#ifndef MOSSCAP_DEFAULTTANK_H
#define MOSSCAP_DEFAULTTANK_H

#include "sim/TankSim.h"

std::unique_ptr<SimulatorBase> simInit() {
    // Set rendering scale
    Renderer::setScale(2.0);

    // Configuration of the robot
    WMRConfig config{2.0 /*kg*/, 0.20 /*m*/, 0.15 /*m*/, 0.04 /*m*/};

    // Create robot
    auto robot = std::make_unique<TankSim>(config);
    robot->setPose(1, 1, 0);

    TOFConfig tofConfig{0, 0, 0};
    tofConfig.boundingBox = BoundingBox(3, 3);

    robot->registerTOF(tofConfig);

    // Configuration of motors
    // In theory you could have a different config for each motor if you wanted
    // But in this case each motor is exactly the same
    const MotorConfig mConfig{3.1, 0.1245, 2.3, 6.0};

    robot->registerLeftMotor(mConfig, 10);
    robot->registerRightMotor(mConfig, 11);

    robot->registerLeftEncoder(1500, 20);
    robot->registerRightEncoder(1500, 21);

    ArduinoRuntime::getInstance().createButton("Button", 1);

    return robot;
}

#endif //MOSSCAP_DEFAULTTANK_H

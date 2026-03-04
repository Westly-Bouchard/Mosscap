//
// Created by Westly Bouchard on 3/2/26.
//

#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <array>

#include <boost/numeric/odeint.hpp>

using stepper_t = boost::numeric::odeint::runge_kutta4<std::array<double, 6>>;

#include "Pose.h"
#include "MecanumPlant.h"
#include "hardware/SimEncoder.h"
#include "hardware/SimMotor.h"

class Simulator {

public:
    Simulator(double chassisMass, double trackWidth, double wheelBase, double wheelMass, double wheelRadius);

    SimMotor& registerFLMotor(std::unique_ptr<SimMotor> m);
    SimMotor& registerFRMotor(std::unique_ptr<SimMotor> m);
    SimMotor& registerBLMotor(std::unique_ptr<SimMotor> m);
    SimMotor& registerBRMotor(std::unique_ptr<SimMotor> m);

    SimEncoder& registerFLEncoder(std::unique_ptr<SimEncoder> e);
    SimEncoder& registerFREncoder(std::unique_ptr<SimEncoder> e);
    SimEncoder& registerBLEncoder(std::unique_ptr<SimEncoder> e);
    SimEncoder& registerBREncoder(std::unique_ptr<SimEncoder> e);

    void setPose(const Pose &p);

    Pose getPose() const;

    void update(double& acc);

private:
    MecanumPlant plant;
    double trackWidth, wheelBase, wheelRadius;
    double simTime;
    std::array<double, 6> state;

    stepper_t stepper;

    std::unique_ptr<SimMotor> FL, FR, BL, BR;
    std::unique_ptr<SimEncoder> FL_encoder, FR_encoder, BL_encoder, BR_encoder;

    constexpr static double dt = 0.002;
};



#endif //SIMULATOR_H

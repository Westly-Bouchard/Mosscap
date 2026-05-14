//
// Created by west on 5/13/26.
//

#ifndef MOSSCAP_TANKSIM_H
#define MOSSCAP_TANKSIM_H

#include "config/MotorConfig.h"
#include "config/TOFConfig.h"
#include "config/WMRConfig.h"
#include "hardware/SimEncoder.h"
#include "hardware/SimMotor.h"
#include "hardware/SimTOF.h"
#include "sim/Simulator.hpp"

class TankSim : public Simulator<6, 2> {
public:
    explicit TankSim(const WMRConfig &config);

    void draw() const override;

    void write() const override;

    void setPose(double x, double y, double theta);

    void registerLeftMotor(MotorConfig c, int pin);
    void registerRightMotor(MotorConfig c, int pin);

    void registerLeftEncoder(int cpr, int pin);
    void registerRightEncoder(int cpr, int pin);

    void registerTOF(TOFConfig c);

protected:
    void updateHardware() override;

    void setPlantInputs() override;

private:
    const WMRConfig config;

    std::unique_ptr<SimMotor> rightMotor;
    std::unique_ptr<SimMotor> leftMotor;

    std::unique_ptr<SimEncoder> leftEncoder;
    std::unique_ptr<SimEncoder> rightEncoder;

    std::unique_ptr<SimTOF> tof;

    [[nodiscard]] std::array<double, 2> fwdKinematics() const;
    [[nodiscard]] std::array<double, 2> bodyToWorld(double x, double y) const;
};



#endif //MOSSCAP_TANKSIM_H

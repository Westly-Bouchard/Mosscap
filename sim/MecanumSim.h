//
// Created by west on 3/22/26.
//

#ifndef INC_441SIM_MECANUMSIM_H
#define INC_441SIM_MECANUMSIM_H

#include <array>

#include "SimulatorNew.h"
#include "../config/MecanumConfig.h"
#include "../hardware/SimMotor.h"
#include "../hardware/SimEncoder.h"

class MecanumSim : public SimulatorNew<6, 4> {
public:
    // Motor and encoder indices to make initialization easier in Sim.h
    static constexpr unsigned int FL{0}, FR{1}, BL{2}, BR{3};

    explicit MecanumSim(const MecanumConfig &config);

    [[nodiscard]] SimMotor& registerMotor(unsigned int idx, std::unique_ptr<SimMotor> motor);
    [[nodiscard]] SimEncoder& registerEncoder(unsigned int idx, std::unique_ptr<SimEncoder> encoder);

protected:
    void updateHardware() override;
    void setPlantInputs() override;

private:
    const MecanumConfig config;

    std::array<std::unique_ptr<SimMotor>, 4> motors;
    std::array<std::unique_ptr<SimEncoder>, 4> encoders;

    [[nodiscard]] std::array<double, 4> fwdKinematics() const;
};


#endif //INC_441SIM_MECANUMSIM_H
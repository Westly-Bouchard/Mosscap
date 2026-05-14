//
// Created by west on 3/21/26.
//

#ifndef INC_441SIM_MECANUMPLANT_H
#define INC_441SIM_MECANUMPLANT_H

#include "Plant.hpp"
#include "config/WMRConfig.h"

/**
 * Dynamics system for a robot with
 * four mecanum wheels
 */
class MecanumPlant final : public Plant<6, 4> {
public:
    /**
     * Construct a plant object with the provided @link WMRConfig
     * @param config Configuration of the system
     */
    explicit MecanumPlant(const WMRConfig& config);

    /**
     * To be used when stepping a system, inherited from @link Plant
     * @param x Current state
     * @param dxdt Derivative of state
     * @param t Current time (unused in this case)
     */
    void operator() (const state_t &x, state_t &dxdt, double t) const override;

private:
    /**
     * Necessary physical constants for force calculations
     */
    const WMRConfig& config;
};

#endif //INC_441SIM_MECANUMPLANT_H
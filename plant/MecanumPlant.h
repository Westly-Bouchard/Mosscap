//
// Created by west on 3/21/26.
//

#ifndef INC_441SIM_MECANUMPLANT_H
#define INC_441SIM_MECANUMPLANT_H

#include <array>

#include "Plant.h"

/**
 * Dynamics system for a robot with
 * four mecanum wheels
 */
class MecanumPlant final : public Plant<6, 4> {
public:
    /**
     * Construct a mecanum plant
     * @param m Mass of the robot in kilograms
     * @param tW Trackwidth in meters
     * @param wB Wheelbase in meters
     * @param wR Wheel radius in meters
     */
    MecanumPlant(double m, double tW, double wB, double wR);

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
    const double wheelRadius, wheelBase, trackWidth;
    const double m_e, I_e;
};


#endif //INC_441SIM_MECANUMPLANT_H
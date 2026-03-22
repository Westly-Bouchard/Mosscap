//
// Created by west on 3/21/26.
//

#ifndef INC_441SIM_PLANT_H
#define INC_441SIM_PLANT_H

#include <array>

using state_t = std::array<double, 6>;

/**
 * Mathematical representation of a system
 * This is essentially a system of equations that represents
 * the system dynamics.
 */
struct Plant {
    virtual ~Plant() = default;

    /**
     * Operator for passing the plant into the ode stepper
     * @param x Current state
     * @param dxdt Derivative of state
     * @param t Current time (usually unused in this context)
     */
    virtual void operator() (const state_t &x, state_t &dxdt, double t) const = 0;
};


#endif //INC_441SIM_PLANT_H
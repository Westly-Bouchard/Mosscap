//
// Created by west on 3/21/26.
//

#ifndef INC_441SIM_PLANT_H
#define INC_441SIM_PLANT_H

#include <array>
#include <vector>

/**
 * Mathematical representation of a system
 * This is essentially a system of equations that represents
 * the system dynamics.
 */
template <unsigned int numStates, unsigned int numInputs>
class Plant {
public:
    using state_t = std::array<double, numStates>;
    using input_t = std::array<double, numInputs>;

    /**
     * Virtual destructor for inheritance reasons
     */
    virtual ~Plant() = default;

    /**
     * Operator for passing the plant into the ode stepper
     * @param x Current state
     * @param dxdt Derivative of state
     * @param t Current time (usually unused in this context)
     */
    virtual void operator() (const state_t &x, state_t &dxdt, double t) const = 0;

    /**
     * Set the inputs to the plant. This is left virtual in case
     * any implementer needs more specific functionality. Or in the
     * case that splitting computations across setInputs and the
     * operator makes more sense semantically
     * @param in Array of inputs to the system
     */
    virtual void setInputs(input_t in) { inputs = in; };

protected:
    /**
     * Current inputs to the system
     */
    input_t inputs;
};


#endif //INC_441SIM_PLANT_H
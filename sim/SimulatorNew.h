//
// Created by west on 3/21/26.
//

#ifndef INC_441SIM_SIMULATORNEW_H
#define INC_441SIM_SIMULATORNEW_H

// Standard library includes
#include <array>
#include <functional>

// Boos includes
#include <boost/numeric/odeint.hpp>

// Project Includes
#include "../ArduinoRuntime.h"
#include "SimulatorBase.h"
#include "../plant/Plant.h"
#include "../hardware/SimClock.h"

/**
 * Simulator base class
 * To be implemented for specific robots (Mecanum, Tank, etc.)
 */
template <unsigned int numStates, unsigned int numInputs>
class SimulatorNew : public SimulatorBase {
public:
    // Why are there so many scopes. So, so many
    using stepper_t = boost::numeric::odeint::runge_kutta4<std::array<double, numStates>>;

    /**
     * Construct simulator, calls Arduino `setup()` function
     * @param timestep Simulation timestep, default is 0.002 (500Hz)
     */
    explicit SimulatorNew(const double timestep=0.002) : dt(timestep), simTime(0.0) {
        // User code setup function
        setup();
    }

    /**
     * Update the simulator (physics and associated hardware)
     * Inherited from SimulatorBase
     * @param acc Accumulated time since last update
     */
    void update(double& acc) override {
        while (acc > dt) {
            // Update hardware
            updateHardware();

            // Call user code loop function
            loop();

            // Update plant inputs
            setPlantInputs();

            // Step the physics sim
            stepper.do_step(std::ref(*plant), state, simTime, dt);
            simTime += dt;

            // Update clock
            clock.updateTime(dt);
            acc -= dt;
        }
    }

protected:
    /**
     * Update the simulated hardware
     * To be implemented for a specific robot
     */
    virtual void updateHardware() = 0;

    /**
     * Update plant inputs (torques, forces, etc.)
     * To be implemented for a specific robot
     */
    virtual void setPlantInputs() = 0;

    /**
     * System dynamics
     */
    std::unique_ptr<Plant<numStates, numInputs>> plant;

    /**
     * Current state of the system, values are:
     * - x      | X position of the robot in the world frame
     * - y      | Y position of the robot in the world frame
     * - theta  | Orientation of the robot measured from world x
     * - v_x    | Velocity of the robot in the world x direction
     * - v_y    | Velocity of the robot in the world y direction
     * - omega  | Angular velocity of hte robot about world z
     */
    Plant<numStates, numInputs>::state_t state{};

    /**
     * Timestep for the simulator
     * The simulator will update at a rate of 1/dt Hz
     */
    const double dt;

private:
    /**
     * The other important state variable for the simulator
     * This is passed to the stepper during the physics update
     */
    double simTime;

    /**
     * Stepper that incrementally integrates the plant and
     * updates the system state
     */
    stepper_t stepper;
};


#endif //INC_441SIM_SIMULATORNEW_H
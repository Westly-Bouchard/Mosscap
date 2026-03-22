//
// Created by west on 3/21/26.
//

#ifndef INC_441SIM_SIMULATORNEW_H
#define INC_441SIM_SIMULATORNEW_H

// Standard library includes
#include <array>

// Boos includes
#include <boost/numeric/odeint.hpp>

// Project Includes
#include "plant/Plant.h"
#include "hardware/SimClock.h"

// Why are there so many scopes. So, so many
using stepper_t = boost::numeric::odeint::runge_kutta4<std::array<double, 6>>;

/**
 * Simulator base class
 * To be implemented for specific robots (Mecanum, Tank, etc.)
 */
class SimulatorNew {
public:
    /**
     * Construct simulator, calls Arduino `setup()` function
     * @param timestep Simulation timestep, default is 0.002 (500Hz)
     */
    explicit SimulatorNew(double timestep=0.002);

    /**
     * Destroy simulator, nothing special here
     */
    virtual ~SimulatorNew() = default;

    /**
     * Update the simulator (physics and associated hardware)
     * @param acc Accumulated time since last update
     */
    void update(double& acc);

    /**
     * Get the clock associated with the simulator.
     * This is called by the ArduinoRuntime to obtain a time handle
     * which it uses to provide the current time to the user code
     * @return Reference to the simulated clock
     */
    static SimClock& getClock();

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
    std::unique_ptr<Plant> plant;

    /**
     * Current state of the system, values are:
     * - x      | X position of the robot in the world frame
     * - y      | Y position of the robot in the world frame
     * - theta  | Orientation of the robot measured from world x
     * - v_x    | Velocity of the robot in the world x direction
     * - v_y    | Velocity of the robot in the world y direction
     * - omega  | Angular velocity of hte robot about world z
     */
    state_t state{};

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

    /**
     * Clock hardware that the ArduinoRuntime uses for the
     * Arduino timing functions
     */
    inline static SimClock clock;

    /**
     * Timestep for the simulator
     * The simulator will update at a rate of 1/dt Hz
     */
    const double dt;
};


#endif //INC_441SIM_SIMULATORNEW_H
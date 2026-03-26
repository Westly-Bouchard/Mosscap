//
// Created by west on 3/23/26.
//

#ifndef INC_441SIM_ISIMULATOR_H
#define INC_441SIM_ISIMULATOR_H
#include "../hardware/SimClock.h"

/**
 * Base class for simulator
 */
class SimulatorBase {
public:
    virtual ~SimulatorBase() = default;

    /**
     * Update the simulator
     * @param acc Accumulated time since last update
     */
    virtual void update(double& acc) = 0;

    /**
     * Draw the simulator state to the current imgui window
     */
    virtual void draw() = 0;

    /**
     * Get the clock associated with the simulator.
     * This is called by the ArduinoRuntime to obtain a time handle
     * which it uses to provide the current time to the user code
     * @return Reference to the simulated clock
     */
    static SimClock& getClock() {
        return clock;
    }

protected:
    /**
     * Clock hardware that the ArduinoRuntime uses for the
     * Arduino timing functions
     */
    inline static SimClock clock;
};

#endif //INC_441SIM_ISIMULATOR_H
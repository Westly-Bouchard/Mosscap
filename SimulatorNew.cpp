//
// Created by west on 3/21/26.
//

#include "SimulatorNew.h"

#include <functional>

#include "ArduinoRuntime.h"

SimulatorNew::SimulatorNew(const double timestep) : simTime(0.0), dt(timestep) {
    // User code setup function
    setup();
}

void SimulatorNew::update(double& acc) {
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

SimClock& SimulatorNew::getClock() {
    return clock;
}
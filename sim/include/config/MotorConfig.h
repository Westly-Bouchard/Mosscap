//
// Created by west on 3/25/26.
//

#ifndef INC_441SIM_MOTORCONFIG_H
#define INC_441SIM_MOTORCONFIG_H

/**
 * Configuration of a simulated brushed DC motor
 */
struct MotorConfig {
    const double kV;
    const double kT;
    const double r;
    const double vBus;

    double staticFriction, kineticFriction;

    /**
     * Construct motor config object
     * @param kV Speed constant of motor in RPM / Volt
     * @param kT Torque constant of motor in Newton Meters per Amp
     * @param r Winding resistance of motor in Ohms
     * @param vBus Nominal system bus voltage
     * @param rotorFriction Optional value (between 0.0 and 1.0) to model internal inefficiencies in the motor
     */
    MotorConfig(const double kV, const double kT, const double r, const double vBus,
                const double rotorFriction = 0) : kV(kV), kT(kT), r(r), vBus(vBus) {
        staticFriction = kT * rotorFriction * vBus / r;
        kineticFriction = staticFriction * 0.8;
    }
};

#endif //INC_441SIM_MOTORCONFIG_H
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

    /**
     * Construct motor config object
     * @param kV Speed constant of motor in RPM / Volt
     * @param kT Torque constant of motor in Newton Meters per Amp
     * @param r Winding resistance of motor in Ohms
     * @param vBus Nominal system bus voltage
     */
    MotorConfig(const double kV, const double kT, const double r, const double vBus) : kV(kV), kT(kT), r(r), vBus(vBus) {}
};

#endif //INC_441SIM_MOTORCONFIG_H
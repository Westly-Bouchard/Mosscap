//
// Created by Westly Bouchard on 4/29/26.
//

#ifndef INC_441SIM_WIRE_H
#define INC_441SIM_WIRE_H

/**
 * Just provides a way for user code to call `Wire.begin()` to maximize
 * compatability.
 */
struct SimWire {
    static void begin() {}
};

#endif //INC_441SIM_WIRE_H

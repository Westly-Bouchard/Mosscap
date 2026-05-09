//
// Created by west on 5/7/26.
//

#ifndef INC_441SIM_DIGITALLYREADABLE_H
#define INC_441SIM_DIGITALLYREADABLE_H

#include "Capability.h"

struct DigitallyReadable : Capability {
    [[nodiscard]] virtual bool digitalRead() const = 0;
};

#endif //INC_441SIM_DIGITALLYREADABLE_H

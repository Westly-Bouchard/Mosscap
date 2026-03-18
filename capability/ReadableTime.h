//
// Created by Westly Bouchard on 3/12/26.
//

#ifndef INC_441SIM_READABLETIME_H
#define INC_441SIM_READABLETIME_H
#include "Capability.h"

struct ReadableTime : Capability {
    [[nodiscard]] virtual double readTime() const = 0;
};

#endif //INC_441SIM_READABLETIME_H

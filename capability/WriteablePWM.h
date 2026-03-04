//
// Created by Westly Bouchard on 3/2/26.
//

#ifndef WRITEABLEPWM_H
#define WRITEABLEPWM_H

#include "Capability.h"

struct WriteablePWM : Capability {
    virtual void writePWM(int value) = 0;
};

#endif //WRITEABLEPWM_H

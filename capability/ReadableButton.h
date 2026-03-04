//
// Created by Westly Bouchard on 3/2/26.
//

#ifndef READABLEBUTTON_H
#define READABLEBUTTON_H

#include "Capability.h"

struct ReadableButton : Capability {
    virtual bool digitalRead() const = 0;
};

#endif //READABLEBUTTON_H

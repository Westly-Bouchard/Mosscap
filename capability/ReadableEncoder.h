//
// Created by Westly Bouchard on 3/2/26.
//

#ifndef READABLEENCODER_H
#define READABLEENCODER_H

#include "Capability.h"

struct ReadableEncoder : Capability {
    virtual int readCount() const = 0;
};

#endif //READABLEENCODER_H

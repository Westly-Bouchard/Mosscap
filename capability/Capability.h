//
// Created by Westly Bouchard on 3/2/26.
//

#ifndef CAPABILITY_H
#define CAPABILITY_H

/**
 * Base class for the interface between a simulator and the Arduino runtime
 */
struct Capability {
    /**
     * Virtual dtor b/c polymorphism
     */
    virtual ~Capability() = default;
};

#endif //CAPABILITY_H

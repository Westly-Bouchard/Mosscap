//
// Created by Westly Bouchard on 3/2/26.
//

#ifndef READABLEBUTTON_H
#define READABLEBUTTON_H

#include "Capability.h"

/**
 * Represents the ability of a piece of simulated hardware to
 * be read as a button.
 */
struct ReadableButton : Capability {
    /**
     *  Analogous to Arduino digitalRead() if pin is attached to a button
     *  To be implemented by an actual piece of hardware
     * @return State of the button, HIGH or LOW
     */
    [[nodiscard]] virtual bool digitalRead() const = 0;
};

#endif //READABLEBUTTON_H

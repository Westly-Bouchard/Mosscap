//
// Created by Westly Bouchard on 3/2/26.
//

#ifndef SIMBUTTON_H
#define SIMBUTTON_H

#include "../capability/ReadableButton.h"

/**
 * Simulated button
 */
class SimButton final : public ReadableButton {
public:
    /**
     * Default ctor
     */
    SimButton() = default;

    /**
     * Get current state of button
     * Overridden from @link ReadableButton
     * @return State of button
     */
    [[nodiscard]] bool digitalRead() const override;

    /**
     * Set the current state of the hardware
     * This is called by the @link Simulator NOT by user code
     * @param s pressed or unpressed
     */
    void setState(bool s);

private:
    /**
     * Current state of the hardware
     */
    bool state{false};
};

#endif //SIMBUTTON_H

//
// Created by west on 5/7/26.
//

#ifndef INC_441SIM_SIMBUTTON_H
#define INC_441SIM_SIMBUTTON_H

#include <atomic>

#include "../capability/DigitallyReadable.h"
#include "../util/Telemetry.h"

class SimButton : public DigitallyReadable, public InputAcceptor {
public:
    explicit SimButton(const std::string &name);

    [[nodiscard]] bool digitalRead() const override;

    void update() override;

private:
    std::string name;

    std::atomic_bool state{false};
};

#endif //INC_441SIM_SIMBUTTON_H

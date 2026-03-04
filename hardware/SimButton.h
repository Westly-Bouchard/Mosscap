//
// Created by Westly Bouchard on 3/2/26.
//

#ifndef SIMBUTTON_H
#define SIMBUTTON_H

#include "../capability/ReadableButton.h"

class SimButton final : public ReadableButton {
public:
    SimButton() = default;

    bool digitalRead() const override;

    void setState(bool state);

private:
    bool state;
};



#endif //SIMBUTTON_H

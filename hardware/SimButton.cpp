//
// Created by west on 5/7/26.
//

#include "SimButton.h"

#include "imgui.h"

SimButton::SimButton(const std::string &name) : InputAcceptor(-1), name(name) {}

bool SimButton::digitalRead() const {
    return state.load();
}

void SimButton::update() {
    if (ImGui::Button(name.c_str())) {
        state.store(true);
    } else {
        state.store(false);
    }
}
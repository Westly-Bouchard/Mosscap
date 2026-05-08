//
// Created by west on 5/7/26.
//

#include "hardware/SimButton.h"

#include <iostream>

#include "imgui.h"

SimButton::SimButton(const std::string &name) : InputAcceptor(-1), name(name) {}

bool SimButton::digitalRead() const {
    return state.load();
}

void SimButton::update() {
    ImGui::Button(name.c_str());
    if (ImGui::IsItemActive()) {
        state.store(true);
    } else {
        state.store(false);
    }
}
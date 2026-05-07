//
// Created by west on 5/6/26.
//

#include "Renderer.h"

#include <algorithm>

using namespace std;

Renderer& Renderer::getInstance() {
    static Renderer instance;
    return instance;
}

void Renderer::registerDrawable(const int layer, Drawable* drawable) {
    // Push object into tracked vector
    drawables.emplace_back(layer, drawable);

    // Sort vector so that objects can be rendered in the correct order
    ranges::sort(drawables, [](const auto& a, const auto& b) {
        return a.first < b.first;
    });
}

void Renderer::registerDrawFunction(const int layer, const function<void()>& func) {
    drawFunctions.emplace_back(layer, func);

    ranges::sort(drawFunctions, [](const auto& a, const auto& b) {
        return a.first < b.first;
    });
}

void Renderer::unregisterDrawable(Drawable* drawable) {
    erase_if(drawables, [drawable](auto d) { return drawable == d.second; });
}

void Renderer::setScale(const double meters) {
    scale = meters;
    pxPerMeter = 800.0 / meters;
}

void Renderer::draw() const {
    // Begin visualization window
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(800, 800));

    ImGui::Begin("Robot", nullptr,
                 ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
                 ImGuiWindowFlags_NoResize);

    // Draw all drawables and drawable functions to the window
    auto it = drawFunctions.begin();

    for (const auto&[layer, d] : drawables) {
        while (it != drawFunctions.end() && it->first < layer) {
            it->second();
            ++it;
        }

        d->draw();
    }

    while (it != drawFunctions.end()) {
        it->second();
        ++it;
    }

    // End the visualization window
    ImGui::End();
    ImGui::PopStyleVar(2);
}
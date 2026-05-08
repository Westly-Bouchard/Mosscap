//
// Created by west on 5/7/26.
//

#include "Telemetry.h"

#include <algorithm>

#include <imgui.h>
#include <ranges>

using namespace std;

Telemetry& Telemetry::getInstance() {
    static Telemetry instance;
    return instance;
}

Telemetry::Telemetry() {
    writing = false;
}

void Telemetry::registerTelemetryProvider(const int priority, TelemetryProvider *provider) {
    providers.emplace_back(priority, provider);

    std::ranges::sort(providers, [](const auto& a, const auto& b) {
        return a.first < b.first;
    });
}

void Telemetry::unregisterTelemetryProvider(TelemetryProvider *provider) {
    erase_if(providers, [provider](const auto& p) {
        return provider == p.second;
    });
}

void Telemetry::write() const {
    // Begin telemetry window
    ImGui::SetNextWindowPos(ImVec2(800, 0));
    ImGui::SetNextWindowSize(ImVec2(480, 800));
    ImGui::Begin("Telemetry", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

    writing = true;

    // Write to window from all providers
    for (const auto &p: providers | views::values) {
        p->write();
    }

    writing = false;

    // Display framerate after all telemetry
    const ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui::Separator();
    ImGui::Text("Average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

    // End telemetry window
    ImGui::End();
}

bool Telemetry::section(const string& name) {
    if (!writing) return false;
    return ImGui::CollapsingHeader(name.c_str(), ImGuiTreeNodeFlags_DefaultOpen);
}

void Telemetry::text(const string& text) {
    if (!writing) return;

    ImGui::Text(text.c_str());
}
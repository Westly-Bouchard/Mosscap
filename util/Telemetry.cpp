//
// Created by west on 5/7/26.
//

#include "Telemetry.h"

#include <algorithm>

#include <imgui.h>
#include <ranges>

using namespace std;

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

    // Write to window from all providers
    for (const auto &p: providers | views::values) {
        p->write();
    }

    // End telemetry window
    ImGui::End();
}
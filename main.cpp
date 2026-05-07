#include <iostream>
#include <optional>
using namespace std;

#include <imgui_impl_glfw.h>
#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h>

#include "graphics/graphics.h"
#include "graphics/Renderer.h"
#include "userCode/Sim.h"

int main() {
    const auto res = setUpRenderer("441Sim");

    GLFWwindow* window;
    if (res.has_value()) {
        window = res.value();
    } else {
        cerr << "Failed to set up renderer" << endl;
        return 1;
    }

    const auto sim = simInit();

    double simAccumulator= 0.0;

    auto lastTime = std::chrono::high_resolution_clock::now();

    while (!glfwWindowShouldClose(window)) {
        /* PHYSICS SIMULATION */
        auto now = std::chrono::high_resolution_clock::now();
        const double frame_dt = std::chrono::duration<double>(now - lastTime).count();
        lastTime = now;

        simAccumulator += frame_dt;

        sim->update(simAccumulator);

        /* RENDERING */
        glfwPollEvents();
        if (glfwGetWindowAttrib(window, GLFW_ICONIFIED) != 0) {
            ImGui_ImplGlfw_Sleep(10);
            continue;
        }

        newFrame();

        // sim->draw();
        Renderer::getInstance().draw();

        ImGui::EndFrame();

        render(window);
    }

    cleanup(window);
    return 0;
}
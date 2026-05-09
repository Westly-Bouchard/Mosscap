#include <iostream>
#include <optional>
using namespace std;

#include <imgui_impl_glfw.h>
#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <functional>
static std::function<void()>            MainLoopForEmscriptenP;
static void MainLoopForEmscripten()     { MainLoopForEmscriptenP(); }
#define EMSCRIPTEN_MAINLOOP_BEGIN       MainLoopForEmscriptenP = [&]() { do
#define EMSCRIPTEN_MAINLOOP_END         while (0); }; emscripten_set_main_loop(MainLoopForEmscripten, 0, true)
#else
#define EMSCRIPTEN_MAINLOOP_BEGIN
#define EMSCRIPTEN_MAINLOOP_END
#endif

#include "util/graphics.h"
#include "util/Renderer.h"
#include "Sim.h"

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

#ifdef __EMSCRIPTEN__
    EMSCRIPTEN_MAINLOOP_BEGIN
#else
    while (!glfwWindowShouldClose(window))
#endif
    {
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

        Renderer::getInstance().draw();

        Telemetry::getInstance().write();

        ImGui::EndFrame();

        render(window);
    }
#ifdef __EMSCRIPTEN__
    EMSCRIPTEN_MAINLOOP_END;
#endif

    cleanup(window);
    return 0;
}
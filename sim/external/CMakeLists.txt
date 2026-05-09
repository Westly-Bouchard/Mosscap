#add_library(imgui imgui/imgui.cpp imgui/imgui_draw.cpp imgui/imgui_widgets.cpp imgui/imgui_tables.cpp imgui/backends/imgui_impl_glfw.cpp imgui/backends/imgui_impl_opengl3.cpp)
#
#target_include_directories(imgui PUBLIC imgui imgui/backends)
#
#target_link_libraries(imgui PUBLIC glfw OpenGL::GL)

add_library(imgui
        imgui/imgui.cpp
        imgui/imgui_draw.cpp
        imgui/imgui_widgets.cpp
        imgui/imgui_tables.cpp
        imgui/backends/imgui_impl_glfw.cpp
        imgui/backends/imgui_impl_opengl3.cpp
)

target_include_directories(imgui PUBLIC imgui imgui/backends)

if(EMSCRIPTEN)
    # Tell ImGui to use Emscripten's built-in GLFW3 port for its headers
    target_compile_options(imgui PUBLIC "-sUSE_GLFW=3")
    target_link_options(imgui PUBLIC "-sUSE_GLFW=3")
else()
    # On desktop, link against the actual fetched/system libraries
    target_link_libraries(imgui PUBLIC glfw OpenGL::GL)
endif()
cmake_minimum_required(VERSION 3.31)

project(441Sim)

set(CMAKE_CXX_STANDARD 23)

find_package(OpenGL REQUIRED)
find_package(glfw3 CONFIG REQUIRED)

find_package(Boost REQUIRED)

add_subdirectory(external)

aux_source_directory(arduino ARDUINO_RUNTIME_SRC)
aux_source_directory(capability CAPABILITIES_SRC)
aux_source_directory(config CONFIG_SRC)
aux_source_directory(util GRAPHICS_SRC)
aux_source_directory(hardware HARDWARE_SRC)
aux_source_directory(plant PLANT_SRC)
aux_source_directory(sim SIM_SRC)
aux_source_directory(userCode USER_SRC)

add_executable(441Sim main.cpp
    ${ARDUINO_RUNTIME_SRC}
    ${CAPABILITIES_SRC}
    ${CONFIG_SRC}
    ${GRAPHICS_SRC}
    ${HARDWARE_SRC}
    ${PLANT_SRC}
    ${SIM_SRC}
    ${USER_SRC}
        capability/ReadableDistance.h
        config/TOFConfig.h
        arduino/VL53L0X.h
        arduino/Wire.h
        util/Renderer.cpp
        util/Renderer.h
        util/Telemetry.cpp
        util/Telemetry.h
        capability/DigitallyReadable.h
        hardware/SimButton.cpp
        hardware/SimButton.h
)

target_include_directories(441Sim PRIVATE ${Boost_INCLUDE_DIRS})

target_link_libraries(441Sim imgui glfw OpenGL::GL)

target_compile_definitions(441Sim PUBLIC "SIM")

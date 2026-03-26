cmake_minimum_required(VERSION 3.31)

project(441Sim)

set(CMAKE_CXX_STANDARD 23)

find_package(OpenGL REQUIRED)
find_package(glfw3 CONFIG REQUIRED)

find_package(Boost REQUIRED)

add_subdirectory(external)

add_executable(441Sim main.cpp
        graphics/graphics.h
        draw.h
        ArduinoRuntime.h
        ArduinoRuntime.cpp
        Handle.h
        Pose.h
        hardware/SimButton.h
        hardware/SimButton.cpp
        hardware/SimEncoder.h
        hardware/SimEncoder.cpp
        hardware/SimMotor.h
        hardware/SimMotor.cpp
        capability/Capability.h
        capability/ReadableButton.h
        capability/ReadableEncoder.h
        capability/WriteablePWM.h
        capability/ReadableTime.h
        arduino/ardMain.cpp
        arduino/Constants.h
        arduino/Motor.h
        arduino/Sim.h
        arduino/Encoder.h
        hardware/SimClock.cpp
        hardware/SimClock.h
        sim/SimulatorNew.cpp
        sim/SimulatorNew.h
        plant/Plant.h
        plant/MecanumPlant.cpp
        plant/MecanumPlant.h
        sim/MecanumSim.cpp
        sim/MecanumSim.h
        config/MecanumConfig.h
        sim/SimulatorBase.h
        config/MotorConfig.h
        graphics/drawHelpers.h
)

target_include_directories(441Sim PRIVATE ${Boost_INCLUDE_DIRS})

target_link_libraries(441Sim imgui glfw OpenGL::GL)

target_compile_definitions(441Sim PUBLIC "SIM")

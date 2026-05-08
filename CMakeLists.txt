cmake_minimum_required(VERSION 3.31)

project(ArduSim)

set(CMAKE_CXX_STANDARD 23)

find_package(OpenGL REQUIRED)
find_package(glfw3 CONFIG REQUIRED)

find_package(Boost REQUIRED)

add_subdirectory(external)

aux_source_directory(src/hardware HARDWARE_SRC)
aux_source_directory(src/plant PLANT_SRC)
aux_source_directory(src/sim SIM_SRC)
aux_source_directory(src/util UTIL_SRC)
aux_source_directory(src/arduino ARDUINO_SRC)

aux_source_directory(userCode USER_SRC)

add_executable(ArduSim src/main.cpp
        ${HARDWARE_SRC}
        ${PLANT_SRC}
        ${SIM_SRC}
        ${UTIL_SRC}
        ${ARDUINO_SRC}
        ${USER_SRC}
)

target_include_directories(ArduSim PRIVATE ${Boost_INCLUDE_DIRS})
target_include_directories(ArduSim PRIVATE "include")
target_include_directories(ArduSim PRIVATE "userCode")

target_link_libraries(ArduSim imgui glfw OpenGL::GL)

target_compile_definitions(ArduSim PUBLIC "SIM")
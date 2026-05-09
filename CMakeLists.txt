cmake_minimum_required(VERSION 3.28.3)

project(ArduSim)

set(CMAKE_CXX_STANDARD 23)

if(NOT EMSCRIPTEN)
    find_package(OpenGL REQUIRED)
    find_package(glfw3 CONFIG REQUIRED)
    find_package(Boost REQUIRED)
endif()

add_subdirectory(external)

# Simulator core files
aux_source_directory(src/hardware HARDWARE_SRC)
aux_source_directory(src/plant PLANT_SRC)
aux_source_directory(src/sim SIM_SRC)
aux_source_directory(src/util UTIL_SRC)
aux_source_directory(src/arduino ARDUINO_SRC)

# User code (Arduino sketch)
aux_source_directory(userCode USER_SRC)

# Simulator core static library
add_library(simcore STATIC
    ${HARDWARE_SRC}
    ${PLANT_SRC}
    ${SIM_SRC}
    ${UTIL_SRC}
    ${ARDUINO_SRC}
)

target_include_directories(simcore PUBLIC
        ${Boost_INCLUDE_DIRS}
        "include"
        "external/imgui"
)

add_executable(ArduSim src/main.cpp ${USER_SRC})

target_include_directories(ArduSim PRIVATE "include")
target_include_directories(ArduSim PRIVATE "userCode")

target_link_libraries(ArduSim simcore imgui)

target_compile_definitions(ArduSim PUBLIC "SIM")

if(EMSCRIPTEN)
    set(EMS
        "-pthread"
        "-sDISABLE_EXCEPTION_CATCHING=1 "
        "-sUSE_BOOST_HEADERS=1 "
        "--use-port=contrib.glfw3"
    )

    set(LDFLAGS
        "-s WASM=1 "
        "-s ALLOW_MEMORY_GROWTH=1 "
        "-s NO_EXIT_RUNTIME=0 "
        "-s ASSERTIONS=1"

        "-s PTHREAD_POOL_SIZE=4 "
        "-s INITIAL_MEMORY=64MB "
    )

    target_compile_options(simcore PRIVATE "${EMS}")

    set_target_properties(ArduSim PROPERTIES SUFFIX ".html")

    target_compile_options(ArduSim PRIVATE "${EMS}")
    target_link_options(ArduSim PRIVATE "${LDFLAGS}" "${EMS}")
else()
    target_link_libraries(ArduSim imgui glfw OpenGL::GL)
endif()
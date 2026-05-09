cmake_minimum_required(VERSION 3.28.3)

project(ArduSim)

set(CMAKE_CXX_STANDARD 23)

if(NOT EMSCRIPTEN)
    find_package(OpenGL REQUIRED)
    find_package(glfw3 CONFIG REQUIRED)
    find_package(Boost REQUIRED)
endif()

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

target_link_libraries(ArduSim imgui)

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

#    set_target_properties(ArduSim PROPERTIES COMPILE_FLAGS "-pthread")
#    set_target_properties(ArduSim PROPERTIES LINK_FLAGS "${WASM_LINK_FLAGS}")
    set_target_properties(ArduSim PROPERTIES SUFFIX ".html")
#
#    target_compile_options(ArduSim PRIVATE "-sUSE_GLFW=3" "-sUSE_BOOST_HEADERS=1")
#    target_link_options(ArduSim PRIVATE "-sUSE_GLFW=3" "-sUSE_BOOST_HEADERS=1")

    target_compile_options(ArduSim PRIVATE "${EMS}")
    target_link_options(ArduSim PRIVATE "${LDFLAGS}" "${EMS}")
else()
    target_link_libraries(ArduSim imgui glfw OpenGL::GL)
endif()
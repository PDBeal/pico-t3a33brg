
cmake_minimum_required(VERSION 3.12)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

project(pico-t3a33brg
    VERSION 1.0.0
    DESCRIPTION "RP2040 library for controlling strips/pixels using T3A33BRG 5mm LEDs using SPI"
)

add_library(pico-t3a33brg INTERFACE)

target_sources(pico-t3a33brg INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/t3a33brg.cpp
)

target_include_directories(pico-t3a33brg INTERFACE ${CMAKE_CURRENT_LIST_DIR})

# Link to pico_stdlib (gpio, time, etc. functions)
target_link_libraries(${PROJECT_NAME} INTERFACE
    pico_stdlib
    hardware_spi
)
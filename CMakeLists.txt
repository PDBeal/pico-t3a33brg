add_library(pico_t3a33brg
        t3a33brg.cpp)

target_link_libraries(pico_t3a33brg
        pico_stdlib
        hardware_spi
        )

target_include_directories (pico_t3a33brg PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
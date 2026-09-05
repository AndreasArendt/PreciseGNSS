cmake_minimum_required(VERSION 3.24)

project(gnss_rtk_lab VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(GNSS_RTK_BUILD_EXAMPLES "Build example programs" ON)
option(GNSS_RTK_BUILD_TESTS "Build test targets" ON)

find_package(GTSAM REQUIRED)

add_library(gnss_rtk STATIC
    src/coordinates/wgs84.cpp
    src/coordinates/transformation.cpp    
    src/core/epoch.cpp    
    src/navigation/kepler_orbit.cpp
    src/rinex/NavData/GalileoNavData.cpp
    src/rinex/NavData/GpsNavData.cpp
    src/rinex/NavData/NavData.cpp
    src/rinex/RinexNavParser.cpp
    src/rinex/RinexObsParser.cpp
    src/rinex/RinexParser.cpp
    src/positioning/providers/broadcast_epoch_provider.cpp
)

target_include_directories(gnss_rtk
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(gnss_rtk
    PUBLIC
        gtsam
)

target_compile_features(gnss_rtk PUBLIC cxx_std_23)

target_compile_options(gnss_rtk
    PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>:/W4 /permissive->
        $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Wconversion>
)

if(GNSS_RTK_BUILD_EXAMPLES)
    add_executable(wgs84_demo
        examples/wgs84_demo.cpp
    )
    target_link_libraries(wgs84_demo PRIVATE gnss_rtk)
endif()

if(GNSS_RTK_BUILD_TESTS)
    enable_testing()

    # Each source file in tests/ is a standalone executable. This keeps small
    # algorithm experiments easy to add without requiring a unit-test framework
    # or another CMake edit. Register only actual automated tests with add_test.
    file(GLOB GNSS_RTK_TEST_SOURCES CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp"
    )

    foreach(test_source IN LISTS GNSS_RTK_TEST_SOURCES)
        get_filename_component(test_target "${test_source}" NAME_WE)
        add_executable("${test_target}" "${test_source}")
        target_link_libraries("${test_target}" PRIVATE gnss_rtk)
    endforeach()

    add_test(NAME wgs84_roundtrip COMMAND wgs84_test)
endif()

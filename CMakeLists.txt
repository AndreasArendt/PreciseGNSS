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
    src/core/abstract_satellite.cpp
    src/core/epoch.cpp
    src/navigation/ephemeris.cpp
    src/navigation/galileo_ephemeris.cpp
    src/navigation/gps_ephemeris.cpp
    src/navigation/kepler_orbit.cpp
    src/rinex/AbstractRinexParser.cpp
    src/rinex/NavData/Galileo/GalileoNavData.cpp
    src/rinex/NavData/Gps/GpsNavData.cpp
    src/rinex/NavData/NavData.cpp
    src/rinex/ObsData/ObsData.cpp
    src/rinex/RinexNavParser/RinexNavParser.cpp
    src/rinex/RinexObsParser/RinexObsParser.cpp
    src/rinex/RinexParser.cpp
    src/rinex/RinexTypes/Satellite.cpp
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

    add_executable(wgs84_test
        tests/wgs84_test.cpp
    )
    target_link_libraries(wgs84_test PRIVATE gnss_rtk)

    add_test(NAME wgs84_roundtrip COMMAND wgs84_test)
endif()

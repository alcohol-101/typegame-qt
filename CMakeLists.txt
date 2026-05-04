cmake_minimum_required(VERSION 3.16)
project(TypeGame VERSION 1.0.0 LANGUAGES CXX)

option(BUILD_TESTS "Build unit tests" ON)

if(DEFINED ENV{CMAKE_IGNORE_PATH})
    list(APPEND CMAKE_IGNORE_PATH "$ENV{CMAKE_IGNORE_PATH}")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

if(WIN32)
    add_compile_options(/utf-8)
endif()

find_package(Qt5 REQUIRED COMPONENTS Core Widgets Multimedia Network Test)

message(STATUS "Qt5 found: ${Qt5_VERSION}")

set(TYPEGAME_RESOURCE ${CMAKE_SOURCE_DIR}/res.qrc)
set(RES_MAIN ${CMAKE_SOURCE_DIR}/res_main.qrc)
set(RES_GAME ${CMAKE_SOURCE_DIR}/res_game.qrc)

add_subdirectory(src)

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()

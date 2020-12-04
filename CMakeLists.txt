cmake_minimum_required(VERSION 3.19)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

project(CommProtoProject)


if (NOT ARDUINO_BOARD)
    add_subdirectory(gtest)
endif()

add_subdirectory(CommProtoLib)

if (ARDUINO_BOARD)
    add_subdirectory(EspApp)
else()
    add_subdirectory(app)
endif()
cmake_minimum_required(VERSION 3.19)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

project(CommProto)


if (NOT ARDUINO_BOARD)
    add_subdirectory(gtest)
    add_subdirectory(libraries/Logging)
    add_subdirectory(CommProto)
    add_subdirectory(app)
else()
    add_subdirectory(libraries/Logging)
    add_subdirectory(CommProto)
    add_subdirectory(EspApp)
endif()

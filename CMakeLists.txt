cmake_minimum_required(VERSION 3.19)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

project(CommProtoProject)

set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/../install")

add_subdirectory(gtest)
add_subdirectory(CommProtoLib)
add_subdirectory(EspApp)
add_subdirectory(app)
add_subdirectory(ServerApp)

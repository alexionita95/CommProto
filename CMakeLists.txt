cmake_minimum_required(VERSION 3.19)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

project(CommProto)


add_subdirectory(gtest)
add_subdirectory(Logger)
add_subdirectory(CommProto)
add_subdirectory(app)

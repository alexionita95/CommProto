cmake_minimum_required(VERSION 3.19)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

project(CommProtoProject)

set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/../install")

set(CMAKE_FOLDER "ThirdParty")
add_subdirectory(gtest)
add_subdirectory(ConcurrentQueue)
add_subdirectory(rapidjson)
add_subdirectory(poco)
set(CMAKE_FOLDER "Libs")
add_subdirectory(CommProtoLib)
set(CMAKE_FOLDER "Embedded")
add_subdirectory(EspApp)
add_subdirectory(app)
set(CMAKE_FOLDER "Runnable")
#add_subdirectory(ServerApp)
add_subdirectory(Dispatch)
add_subdirectory(Endpoint)
add_subdirectory(UXService)

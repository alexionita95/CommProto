project (SocketLib)

if(UNIX)
    SET(PD_BASE "${PROJECT_SOURCE_DIR}/pd/unix")
endif()

SET(PD_INTERFACE_FOLDER "${PD_BASE}/interface")
SET(PD_SRC_FOLDER "${PD_BASE}/src")

file(GLOB_RECURSE PROJECT_SRC
        "${PD_INTERFACE_FOLDER}/*.cpp"
        "${PD_INTERFACE_FOLDER}/*.h"
        "${PD_SRC_FOLDER}/*.cpp"
        "${PD_SRC_FOLDER}/*.h"
        interface/*.cpp
        interface/*.h
        src/*.cpp
        src/*.h
        )


add_library(${PROJECT_NAME} STATIC ${PROJECT_SRC})

target_include_directories(${PROJECT_NAME} INTERFACE interface ${PD_INTERFACE_FOLDER})
include_directories(interface)

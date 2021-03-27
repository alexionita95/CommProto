project(rapidjson)

file(GLOB_RECURSE PROJECT_SRC RELATIVE
        "${CMAKE_CURRENT_SOURCE_DIR}"
        include/*.h
        )

add_library(${PROJECT_NAME} INTERFACE
    ${PROJECT_SRC}
)

target_include_directories(${PROJECT_NAME} INTERFACE include)
project(Logger)

add_library(${PROJECT_NAME} STATIC src/Logging.h src/Logging.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC src)

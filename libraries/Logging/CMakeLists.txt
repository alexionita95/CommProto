project(Logging)


add_library(${PROJECT_NAME} STATIC Logging.h Logging.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC .)
if(ARDUINO_BOARD)
    target_link_arduino_libraries(${PROJECT_NAME} PRIVATE core)
endif()

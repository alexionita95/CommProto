project(EspApp CXX)

add_executable(${PROJECT_NAME} source.cpp)

# Arduino.h is included in hello_world.cpp, so link with Arduino core
target_link_arduino_libraries(${PROJECT_NAME} PRIVATE core)

# This is needed for the generation of HEX binary and uploading it
target_enable_arduino_upload(${PROJECT_NAME})

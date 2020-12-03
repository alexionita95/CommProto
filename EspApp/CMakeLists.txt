project(EspApp CXX)

list(APPEND ARDUINO_LIBRARIES_SEARCH_PATHS_EXTRA
        "${CMAKE_CURRENT_SOURCE_DIR}/..")

add_executable(${PROJECT_NAME} source.cpp)

# Arduino.h is included in hello_world.cpp, so link with Arduino core
target_link_arduino_libraries(${PROJECT_NAME} PRIVATE core Logging CommProto ESP8266WiFi FastLED)

include_directories(${CMAKE_SOURCE_DIR}/libraries/CommProto/interface ${CMAKE_SOURCE_DIR}/libraries/CommProto/pd/esp/interface ${CMAKE_SOURCE_DIR}/libraries/CommProto)

# This is needed for the generation of HEX binary and uploading it
target_enable_arduino_upload(${PROJECT_NAME})

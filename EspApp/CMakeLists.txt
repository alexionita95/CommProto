project(EspApp)


set(ESPAPP_SOURCE 
SDKWrapper.h
DeviceWrapper.h
DeviceWrapper.cpp
source.cpp)

add_executable(${PROJECT_NAME} ${ESPAPP_SOURCE})
target_link_libraries(${PROJECT_NAME} PRIVATE CommProto)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories("../CommProtoLib/src")


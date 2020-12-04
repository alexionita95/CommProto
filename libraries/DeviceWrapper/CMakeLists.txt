project(DeviceWrapper)


add_library(${PROJECT_NAME} DeviceWrapper.h DeviceWrapper.cpp SDKWrapper.h)

target_link_libraries(${PROJECT_NAME} Logging.h CommProto)

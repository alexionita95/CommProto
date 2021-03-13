project(ConcurrentQueue)

add_library(${PROJECT_NAME} INTERFACE
    moodycamel/concurrentqueue.h
)

target_include_directories(${PROJECT_NAME} INTERFACE moodycamel)
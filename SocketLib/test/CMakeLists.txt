project(SocketLibTest)

FILE(GLOB_RECURSE TEST_SRC *.cpp *.h)
add_executable(${PROJECT_NAME} ${TEST_SRC})
target_link_libraries(${PROJECT_NAME}  gtest_main SocketLib)
add_test(NAME example_test COMMAND example)
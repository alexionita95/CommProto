project(Serverapp)

include (${CMAKE_SOURCE_DIR}/cmake/utility.cmake)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


set(CMAKE_PREFIX_PATH "C:/Qt/5.10.0/msvc2015_64")

find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_executable(${PROJECT_NAME} WIN32 
    mainwindow.ui
    mainwindow.cpp
    main.cpp
)

target_link_libraries(${PROJECT_NAME} Qt5::Widgets)

set(INSTALL_DIR "${CMAKE_BINARY_DIR}/../install")

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX})

message( "$<TARGET_FILE:${PROJECT_NAME}>")

find_program(WINDEPLOYQT_EXECUTABLE NAMES windeployqt HINTS ${QTDIR} ENV QTDIR PATH_SUFFIXES bin)
add_custom_command(TARGET ${PROJECT_NAME}
        POST_BUILD
        COMMAND ${WINDEPLOYQT_EXECUTABLE} --dir ${CMAKE_INSTALL_PREFIX} "$<TARGET_FILE:${PROJECT_NAME}>"
    )


set_executable_path(${PROJECT_NAME}) 
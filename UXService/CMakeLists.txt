project(UXService)

find_package(Poco "1.10.1" REQUIRED COMPONENTS Net Util)

include (${CMAKE_SOURCE_DIR}/cmake/utility.cmake)

add_executable(${PROJECT_NAME} 
    main.cpp
)

target_link_libraries(${PROJECT_NAME} CommProto Poco::Net Poco::Util)

include_directories(.)

set(INSTALL_DIR "${CMAKE_BINARY_DIR}/../install")

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX})

IF(WIN32)
message("win32 TYPE=${CMAKE_BUILD_TYPE}")
		  install(FILES 
			"${Poco_DIR}/../bin/PocoUtild.dll"   
			"${Poco_DIR}/../bin/PocoNetd.dll"   
			"${Poco_DIR}/../bin/PocoFoundationd.dll"   
			"${Poco_DIR}/../bin/PocoJSONd.dll"   
			"${Poco_DIR}/../bin/PocoXMLd.dll"   
		  DESTINATION 
			"${INSTALL_DIR}")
else()

endif()


##set_executable_path(${PROJECT_NAME}) 
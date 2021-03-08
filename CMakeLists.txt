project("jarrun")
cmake_minimum_required(VERSION 3.7)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(${PROJECT_NAME} STATIC
	src/download.cpp
	src/execjar.cpp
)
set_target_properties(${PROJECT_NAME} PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR}/src)

IF(NOT WIN32)
	find_package(JNI REQUIRED)
	target_link_libraries(${PROJECT_NAME} PRIVATE ${JNI_LIBRARIES})
ENDIF()

IF(WIN32)
	find_package(CURL CONFIG REQUIRED)
ELSE()
	find_package(CURL COMPONENTS HTTPS REQUIRED)
ENDIF()
target_link_libraries(${PROJECT_NAME} PRIVATE ${CURL_LIBRARIES})

if(BUILD_TESTING)
	add_executable(test src/test.cpp)
	target_link_libraries(test PRIVATE ${PROJECT_NAME})
endif()

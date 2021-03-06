cmake_minimum_required(VERSION 3.9 FATAL_ERROR)

project(sf2 VERSION 1.0 LANGUAGES CXX)

file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/dummy.cpp "#include <sf2/sf2.hpp>
")
add_library(sf2 STATIC
	${CMAKE_CURRENT_BINARY_DIR}/dummy.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/include/sf2/formats/json_reader.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/include/sf2/formats/json_writer.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/include/sf2/reflection.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/include/sf2/reflection_data.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/include/sf2/serializer.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/include/sf2/sf2.hpp)

target_include_directories(sf2 PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:include>)
	
target_compile_features(sf2 PUBLIC cxx_std_17)

install(TARGETS sf2 EXPORT sf2_targets ARCHIVE DESTINATION lib INCLUDES DESTINATION include)
install(
    DIRECTORY ${CMAKE_SOURCE_DIR}/include/
    DESTINATION include
    FILES_MATCHING PATTERN "*.h*")
install(EXPORT sf2_targets
	FILE sf2.cmake
	NAMESPACE sf2::
	DESTINATION lib/cmake/sf2
)

option(SF2_BUILD_TESTS "Build tests" off)

if(SF2_BUILD_TESTS)
	enable_testing()

	MESSAGE("Building tests for sf2")

	if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
		message(STATUS "Setting build type to 'Debug' as none was specified.")
		set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
	endif()

	add_executable(sf2_test_simple "tests/test_simple.cpp")
	target_link_libraries(sf2_test_simple PRIVATE sf2)
	add_executable(sf2_test_advanced "tests/test_advanced.cpp")
	target_link_libraries(sf2_test_advanced PRIVATE sf2)

	add_test(NAME simple_usage   COMMAND sf2_test_simple)
	add_test(NAME advanced_usage COMMAND sf2_test_advanced)
endif()

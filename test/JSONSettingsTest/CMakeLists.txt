cmake_minimum_required(VERSION 3.2)

# Enable testing module
enable_testing()

# Add project folder into includes
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Find external dependencides
find_package(gtest)

# Configure test project
set(JSON_SETTINGS_TEST_PROJECT JSONSettingsTest)
file(GLOB_RECURSE JSON_SETTINGS_TEST_PROJECT_SRC "*.cpp")
file(GLOB_RECURSE JSON_SETTINGS_TEST_PROJECT_HDR "*.h")
add_executable(${JSON_SETTINGS_TEST_PROJECT} ${JSON_SETTINGS_TEST_PROJECT_SRC} ${JSON_SETTINGS_TEST_PROJECT_HDR})
target_link_libraries(${JSON_SETTINGS_TEST_PROJECT} JSONSettings gtest::gtest)

# Ignored missing PDBs link warning on Visual Studio
if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
	set_target_properties(${JSON_SETTINGS_TEST_PROJECT} PROPERTIES LINK_FLAGS "/ignore:4099")
endif()

#Configure source groups
foreach(FILE ${JSON_SETTINGS_TEST_PROJECT_SRC} ${JSON_SETTINGS_TEST_PROJECT_HDR}) 
    get_filename_component(PARENT_DIR "${FILE}" DIRECTORY)
    string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}" "" GROUP "${PARENT_DIR}")
    string(REPLACE "/" "\\" GROUP "${GROUP}")

    if ("${FILE}" MATCHES ".*\\.cpp")
       set(GROUP "Source Files${GROUP}")
    elseif("${FILE}" MATCHES ".*\\.h")
       set(GROUP "Header Files${GROUP}")
    endif()

    source_group("${GROUP}" FILES "${FILE}")
endforeach()

# Register tests
add_test(NAME JSONSettingsTest COMMAND ${JSON_SETTINGS_TEST_PROJECT})

# Additional target for code coverage analysis
if (CMAKE_BUILD_TYPE STREQUAL "Coverage")
	include(CodeCoverage)
	setup_target_for_coverage(JSONSettingsTestCoverage JSONSettingsTest coverage)
endif()

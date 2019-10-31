cmake_minimum_required(VERSION 3.2)

project(JSONSettingsPackageTestProject)

# Configure environment
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../cmake ${CMAKE_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Find tested package
find_package(JSONSettings)

# Configure test package project
set(JSON_SETTINGS_PACKAGE_TEST_PROJECT JSONSettingsPackageTest)
add_executable(${JSON_SETTINGS_PACKAGE_TEST_PROJECT} JSONSettingsExample.cpp)
target_link_libraries(${JSON_SETTINGS_PACKAGE_TEST_PROJECT} JSONSettings::JSONSettings)

# Register test
enable_testing()
add_test(NAME JSONSettingsPackageTest COMMAND ${JSON_SETTINGS_PACKAGE_TEST_PROJECT})

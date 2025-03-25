cmake_minimum_required(VERSION 3.2)

project(JSONSettingsRESTAPIPackageTestProject)

# Configure environment
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../cmake ${CMAKE_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Find tested package
find_package(JSONSettingsRESTAPI)
find_package(RapidJSONAdapter)

# Configure test package project
set(JSON_SETTINGS_REST_API_PACKAGE_TEST_PROJECT JSONSettingsRESTAPIPackageTest)
add_executable(${JSON_SETTINGS_REST_API_PACKAGE_TEST_PROJECT} JSONSettingsRESTAPIExample.cpp)
target_link_libraries(${JSON_SETTINGS_REST_API_PACKAGE_TEST_PROJECT}
                      JSONSettingsRESTAPI::JSONSettingsRESTAPI
                      RapidJSONAdapter::RapidJSONAdapter)

# Register test
enable_testing()
add_test(NAME JSONSettingsRESTAPIPackageTest COMMAND ${JSON_SETTINGS_REST_API_PACKAGE_TEST_PROJECT})

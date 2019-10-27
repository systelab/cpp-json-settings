cmake_minimum_required(VERSION 3.2)

# Find external dependencies
find_package(RapidJSONAdapter)

# Configure preprocessor definitions
add_compile_options(-DRAPIDJSON_HAS_STDSTRING=1)

# Add project folder into includes
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Configure JSONSettings static library
set(JSON_SETTINGS JSONSettings)
file(GLOB_RECURSE JSON_SETTINGS_SRC "*.cpp")
file(GLOB_RECURSE JSON_SETTINGS_HDR "*.h")
add_library(${JSON_SETTINGS} STATIC ${JSON_SETTINGS_SRC} ${JSON_SETTINGS_HDR})
target_link_libraries(${JSON_SETTINGS} RapidJSONAdapter::RapidJSONAdapter)

#Configure source groups
foreach(FILE ${JSON_SETTINGS_SRC} ${JSON_SETTINGS_HDR}) 
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

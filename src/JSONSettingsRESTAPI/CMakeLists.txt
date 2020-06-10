cmake_minimum_required(VERSION 3.2)

project(JSONSettingsRESTAPI)

# Configure environment
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Add project folder into includes
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Find external dependencies
find_package(RESTAPICore)
find_package(JSONSettings)

# Configure JSON settings test utilities project
set(JSON_SETTINGS_REST_API_PROJECT JSONSettingsRESTAPI)
file(GLOB_RECURSE JSON_SETTINGS_REST_API_PROJECT_SRC "*.cpp")
file(GLOB_RECURSE JSON_SETTINGS_REST_API_PROJECT_HDR "*.h")

set(EXCLUDE_DIRS test_package build)
foreach (EXCLUDE_DIR ${EXCLUDE_DIRS})

	foreach (TMP_PATH ${JSON_SETTINGS_REST_API_PROJECT_SRC})
		string (FIND ${TMP_PATH} ${EXCLUDE_DIR} EXCLUDE_DIR_FOUND)
		if (NOT ${EXCLUDE_DIR_FOUND} EQUAL -1)
			list (REMOVE_ITEM JSON_SETTINGS_REST_API_PROJECT_SRC ${TMP_PATH})
		endif ()
	endforeach(TMP_PATH)

	foreach (TMP_PATH ${JSON_SETTINGS_REST_API_PROJECT_HDR})
		string (FIND ${TMP_PATH} ${EXCLUDE_DIR} EXCLUDE_DIR_FOUND)
		if (NOT ${EXCLUDE_DIR_FOUND} EQUAL -1)
			list (REMOVE_ITEM JSON_SETTINGS_REST_API_PROJECT_HDR ${TMP_PATH})
		endif ()
	endforeach(TMP_PATH)

endforeach(EXCLUDE_DIR)



add_library(${JSON_SETTINGS_REST_API_PROJECT} STATIC ${JSON_SETTINGS_REST_API_PROJECT_SRC} ${JSON_SETTINGS_REST_API_PROJECT_HDR})
target_link_libraries(${JSON_SETTINGS_REST_API_PROJECT}
					  JSONSettings::JSONSettings
					  RESTAPICore::RESTAPICore)

# Configure source groups
foreach(FILE ${JSON_SETTINGS_REST_API_PROJECT_SRC} ${JSON_SETTINGS_REST_API_PROJECT_HDR})
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

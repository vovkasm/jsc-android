cmake_minimum_required(VERSION 3.24)

set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
set(CMAKE_POLICY_DEFAULT_CMP0126 NEW)

project(JscAndroid)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_CONFIGURATION_TYPES)
    if(NOT CMAKE_BUILD_TYPE)
        message(STATUS "Setting build type to 'Release' as none was specified.")
        set_property(CACHE CMAKE_BUILD_TYPE PROPERTY VALUE "Release")
    endif()
endif()

add_subdirectory(icu/source)

set(ENABLE_API_TESTS OFF)
set(ENABLE_TOOLS OFF)
set(DEVELOPER_MODE OFF)
set(PORT JSCOnly)
set(ENABLE_WEBASSEMBLY OFF)
add_subdirectory(webkit)

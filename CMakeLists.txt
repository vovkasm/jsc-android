cmake_minimum_required(VERSION 3.24)

set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
set(CMAKE_POLICY_DEFAULT_CMP0126 NEW)
# Honor visibility properties for all target types.
set(CMAKE_POLICY_DEFAULT_CMP0063 NEW)

project(JscAndroid C CXX)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_C_VISIBILITY_PRESET hidden)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN ON)

set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
set(LTO_MODE "thin")

if(NOT CMAKE_CONFIGURATION_TYPES)
    if(NOT CMAKE_BUILD_TYPE)
        message(STATUS "Setting build type to 'Release' as none was specified.")
        set_property(CACHE CMAKE_BUILD_TYPE PROPERTY VALUE "Release")
    endif()
endif()

add_subdirectory(icu/source)

set(LIB_INSTALL_DIR "lib/${CMAKE_ANDROID_ARCH_ABI}")
set(ENABLE_API_TESTS OFF)
set(ENABLE_TOOLS OFF)
set(DEVELOPER_MODE OFF)
set(PORT JSCOnly)
set(ENABLE_WEBASSEMBLY OFF)
set(ENABLE_WEBASSEMBLY_B3JIT OFF)
add_subdirectory(webkit)

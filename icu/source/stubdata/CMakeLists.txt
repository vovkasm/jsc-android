
file(READ sources.txt SOURCES)
string(REGEX REPLACE "\n" ";" SOURCES ${SOURCES})

add_library(data ${SOURCES})
add_library(ICU::data ALIAS data)

target_include_directories(data PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../common)

target_compile_definitions(data PRIVATE U_COMMON_IMPLEMENTATION=1 U_STATIC_IMPLEMENTATION=1)
target_compile_features(data PRIVATE cxx_std_17)
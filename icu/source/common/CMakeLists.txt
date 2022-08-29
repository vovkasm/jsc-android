
file(READ sources.txt SOURCES)
string(REGEX REPLACE "\n" ";" SOURCES ${SOURCES})

add_library(uc ${SOURCES})
add_library(ICU::uc ALIAS uc)

target_include_directories(uc
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

target_compile_definitions(uc PRIVATE U_COMMON_IMPLEMENTATION=1)
target_compile_features(uc PRIVATE cxx_std_17)
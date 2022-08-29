
file(READ sources.txt SOURCES)
string(REGEX REPLACE "\n" ";" SOURCES ${SOURCES})

add_library(i18n ${SOURCES})
add_library(ICU::i18n ALIAS i18n)

target_include_directories(i18n
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

target_compile_definitions(i18n PRIVATE U_I18N_IMPLEMENTATION=1)
target_compile_features(i18n PRIVATE cxx_std_17)
target_link_libraries(i18n uc data)
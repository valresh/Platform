if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
endif()

set(CMAKE_FOLDER ".RSUs/HoneyWell5xx/Utils")

add_compile_definitions(__STDC_LIMIT_MACROS)
if(UNICODE)
add_compile_options(/D "_UNICODE" /D "UNICODE")
else()
add_compile_options($<$<C_COMPILER_ID:MSVC>:/utf-8>)
add_compile_options($<$<CXX_COMPILER_ID:MSVC>:/utf-8>)
endif()
add_compile_options(/EHsc)


################################################################################
# Output directory
################################################################################
set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_DIRECTORY_DEBUG   "${CMAKE_CURRENT_SOURCE_DIR}/../../../../temp/"
    OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_SOURCE_DIR}/../../../../temp/"
)

################################################################################
# Include directories
################################################################################
set(COMMON_INCLUDES
    "${CMAKE_CURRENT_SOURCE_DIR}/"
    "${CMAKE_CURRENT_SOURCE_DIR}/../include/"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../common/include/"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../system/include/"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../crosslib/include/"
)

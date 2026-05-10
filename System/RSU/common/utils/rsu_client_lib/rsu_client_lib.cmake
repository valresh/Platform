set(PROJECT_NAME rsu_client_lib)

set(Impl
	"rsu_client_lib.cpp"
)
source_group("Impl" FILES ${Impl})

set(Include
	"../../include/rsu_client_lib.h"
)
source_group("Include" FILES ${Include})

set(ALL_FILES
    ${Impl}
    ${Include}
)

add_compile_definitions(RSU_CLIENT_LIB_EXPORT)
add_compile_options($<$<C_COMPILER_ID:MSVC>:/utf-8>)
add_compile_options($<$<CXX_COMPILER_ID:MSVC>:/utf-8>)

add_library (${PROJECT_NAME} SHARED ${ALL_FILES})

#add_dependencies (${PROJECT_NAME} crc32)
target_link_libraries(${PROJECT_NAME} crc32)

if(NOT MSVC)
target_compile_options(${PROJECT_NAME} PRIVATE -fPIC)
endif()

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
endif()

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
target_include_directories(${PROJECT_NAME} PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../include/"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../../system/include/"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../../crosslib/include/"
)

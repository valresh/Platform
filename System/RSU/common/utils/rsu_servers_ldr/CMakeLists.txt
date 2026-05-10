cmake_minimum_required (VERSION 3.10)
set(CMAKE_FOLDER ".RSUs/common/utils")

set(PROJECT_NAME rsu_servers_ldr)
include(../../common.cmake)

set(Impl
	"rsu_servers_ldr.cpp"
)
source_group("Impl" FILES ${Impl})

set(ALL_FILES
    ${Impl}
    "../../../../utf8.manifest"
)

add_compile_definitions(RSU_SERVER_EXPORT)
add_library (${PROJECT_NAME} SHARED ${ALL_FILES})
target_link_libraries( ${PROJECT_NAME} PUBLIC crc32 CommonRSUs Utils)

target_include_directories(${PROJECT_NAME} PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../include/"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../../system/include/"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../../crosslib/include/"
)




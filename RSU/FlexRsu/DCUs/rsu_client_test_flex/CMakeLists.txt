cmake_minimum_required (VERSION 3.10)
set(PROJECT_NAME rsu_client_test_flex)
include (${CMAKE_CURRENT_SOURCE_DIR}/../../FlexCommon.cmake)

set(Impl
	"rsu_client_test_flex.cpp"
)
source_group("Impl" FILES ${Impl})

set(ALL_FILES
    ${Impl}
    "../../../../utf8.manifest"
)

add_executable (${PROJECT_NAME} ${ALL_FILES} )

target_link_libraries(${PROJECT_NAME} PUBLIC Utils PRIVATE CommonRSUs PRIVATE Connect PRIVATE crosslib)


set(PROJECT_INCLUDES
        "${CMAKE_CURRENT_SOURCE_DIR}/"        
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${PROJECT_INCLUDES}
    ${COMMON_INCLUDES}
)


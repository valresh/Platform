cmake_minimum_required (VERSION 3.10)
set(PROJECT_NAME FlexRsu_Client)
include (../../FlexCommon.cmake)

set(Headers
    "CBsPool.h"
    "Client.h"
    "ICBsPool.h"
)
source_group("Headers" FILES ${Headers})

set(Sources
    "ACSsupport.cpp"
    "CBsPool.cpp"
    "Client.cpp"
    "FlexRsu_Client_dll.cpp"
    "flex_client_lib.cpp"
    "RsuConnectionImpl.cpp"
)
source_group("Sources" FILES ${Sources})

set(ALL_FILES
    ${Headers}
    ${Sources}
    "../../../../utf8.manifest"
)

add_library(${PROJECT_NAME} SHARED ${ALL_FILES})

add_compile_definitions(RSU_CLIENT_LIB_EXPORT)

target_link_libraries(${PROJECT_NAME} 
    PUBLIC FlexRsuCommon
    PUBLIC FlexRsuList
    PUBLIC FlexRsuImpl
    PRIVATE Connect 
    PRIVATE Utils
    PRIVATE crc32
)

set(ADDITIONAL_LIBRARY_DEPENDENCIES
        "Utils"
        "RsuX"
        "FlexRsuImpl"
)

target_link_libraries(${PROJECT_NAME} PUBLIC "${ADDITIONAL_LIBRARY_DEPENDENCIES}")

target_include_directories(${PROJECT_NAME} PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../include/"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../../system/include/"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../../crosslib/include/"
)


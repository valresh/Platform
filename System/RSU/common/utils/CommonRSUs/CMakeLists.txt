cmake_minimum_required (VERSION 3.10)
set(CMAKE_FOLDER ".RSUs/common/utils")
set(PROJECT_NAME CommonRSUs)
include(../../common.cmake)

set(Header_Files
    "../../include/CommonRSUs.h"
    "../../include/hashRSU.h"
    "OnDiskAllocator.h"
    "../../include/rsuClipboard.h"
    "../../include/rsuErr.h"
    "../../include/rsuFormats.h"
    "../../include/rsuIsType.h"
    "../../include/rsuMemoryManager.h"
    "../../include/rsuNoNames.h"
    "../../../../system/include/rsus/rsuPaths.h"
    "../../include/rsuRand.h"
    "../../include/rsuStr.h"
)
source_group("Header Files" FILES ${Header_Files})

set(Impl
    "CommonRSUs.cpp"
    "hashRSU.cpp"
    "OnDiskAllocator.cpp"
    "rsuClipboard.cpp"
    "CRsuLogFile.cpp"
    "rsuErr.cpp"
    "rsuFormats.cpp"
    "rsuIsType.cpp"
    "rsuMemoryManager.cpp"
    "rsuNoNames.cpp"
    "rsuNoNames2D.cpp"
    "rsuPath.cpp"
    "rsuProjectVersion.cpp"
    "rsuRand.cpp"
    "rsuStr.cpp"
    "CommonRSUs_dll.cpp"
)
source_group("Impl" FILES ${Impl})

set(ALL_FILES
    ${Header_Files}
    ${Impl}
    "../../../../utf8.manifest"
)

add_compile_definitions(__STDC_LIMIT_MACROS)
add_compile_definitions(COMMONRSUS_EXPORTS)

add_library(${PROJECT_NAME} SHARED ${ALL_FILES})

add_dependencies (${PROJECT_NAME} cutf )

target_link_directories(${PROJECT_NAME} PUBLIC
    "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}"
)

target_link_libraries(${PROJECT_NAME} zlibvc crosslib)
target_include_directories(${PROJECT_NAME} PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../include/"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../../system/include/"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../../crosslib/include/"
)

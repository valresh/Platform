cmake_minimum_required (VERSION 3.10)
set(PROJECT_NAME FlexRsuAlarmMsg)
include (../../FlexCommon.cmake)

set(Headers
	"./PrivateVars/DAlrmGen.h"
	"./PrivateVars/DigAlrm.h"
	"./PrivateVars/Hys.h"
	"./PrivateVars/M2U.h"
	"./PrivateVars/RECMSG.h"
)
source_group("Headers" FILES ${Headers})

set(Sources
	"AnAlrm.cpp"
	"DAlrmGen.cpp"
	"DAlrmOff.cpp"
	"DAlrmOn.cpp"
	"FlexRsuAlarmMsg_dll.cpp"
	"Hys.cpp"
	"M2UOff.cpp"
	"M2UOn.cpp"
	"RECMSG.cpp"
	"TXTSET.cpp"
)
source_group("Sources" FILES ${Sources})

set(ALL_FILES
    ${Headers}
    ${Sources}
    "../../../../utf8.manifest"
)

add_library(${PROJECT_NAME} SHARED ${ALL_FILES})

add_compile_definitions(FLEXRSUALARMSKERNEL_EXPORTS)

target_link_libraries(${PROJECT_NAME} 
    PUBLIC FlexRsuCommon CommonRSUs
	PUBLIC FlexRsuCore
	PUBLIC FlexRsuConnect
    PRIVATE Utils
    PRIVATE crc32
)

target_include_directories(${PROJECT_NAME} PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../include/"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../../system/include/"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../../crosslib/include/"
    "${CMAKE_CURRENT_SOURCE_DIR}/../FlexRsuCore/"
)


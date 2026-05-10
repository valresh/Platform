set(PROJECT_NAME TrendsRSU_Server)
set(CMAKE_FOLDER ".RSUs/common/utils")

include(../../common.cmake)

set(Headers
	"TrendsRSU_Server.h"
)
source_group("Headers" FILES ${Headers})

set(Sources
	"TrendsRSU_Server.cpp"
	"trends_rsu_server_lib.cpp"
)
source_group("Sources" FILES ${Sources})

set(ALL_FILES
    ${Headers}
    ${Sources}
    "../../../../utf8.manifest"
)

add_library(${PROJECT_NAME} SHARED ${ALL_FILES})
add_compile_definitions(RSU_SERVER_LIB_EXPORT)
include_directories(
	"${CMAKE_CURRENT_SOURCE_DIR}/../../common/include/"
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE CommonRSUs
    PRIVATE crc32
    PUBLIC Utils
)

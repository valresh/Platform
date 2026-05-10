project(TrendsKernel)
set(CMAKE_FOLDER ".RSUs/common/utils")

include(../../common.cmake)

include_directories(
	"${CMAKE_SOURCE_DIR}/system/include/rsus/"
)

set(Headers
    "../../include/TrendSrvH.h"
    "../../include/TrendStr.h"
    "TrendsRsuKernel.h"
)
source_group("Headers" FILES ${Headers})

set(Sources
    "TrendsKernel_dll.cpp"
    "trendsrsukernel-file.cpp"
    "TrendsRsuKernel-Fill.cpp"
    "TrendsRsuKernel.cpp"
)
source_group("Sources" FILES ${Sources})

set(ALL_FILES ${Headers} ${Sources})
add_compile_definitions(TRENDSKERNEL_EXPORTS)
add_library(${PROJECT_NAME} SHARED ${ALL_FILES})

target_link_libraries(${PROJECT_NAME} PUBLIC
    CommonRSUs
)

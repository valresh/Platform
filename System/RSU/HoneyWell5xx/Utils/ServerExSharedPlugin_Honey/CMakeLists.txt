cmake_minimum_required (VERSION 3.10) 
set(CMAKE_FOLDER ".RSUs/HoneyWell5xx/Utils")

set(PROJECT_NAME ServerExSharedPlugin_Honey)
include(../../../../global.cmake)

set(Headers
	"EventStringH.h"
	"LocalUtils.h"
	"ServerExSharedPlugin_Honey.h"
	"QB_UserDefName.h"
)
source_group("Headers" FILES ${Headers})

set(Sources
    "alarmsext.cpp"
	"EventStringH.cpp"
	"FindConnPointEx.cpp"
	"FindHoney.cpp"
	"LocalUtils.cpp"
	"PutModel.cpp"
    "ServerExSharedPlugin_Honey_dll.cpp"
)
source_group("Sources" FILES ${Sources})	

set(ALL_FILES
    ${Headers}
    ${Sources}
    "../../../../utf8.manifest"
)
add_library(${PROJECT_NAME} SHARED ${ALL_FILES})

target_link_directories(${PROJECT_NAME} PUBLIC
    "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}"
)

target_link_libraries(${PROJECT_NAME} PRIVATE CommonRSUs PRIVATE Utils PRIVATE UtilsH5xx PRIVATE UtilsSM5xx PRIVATE UtilsQB5xx PRIVATE ConnectH5xx PRIVATE ConnectSM5xx PRIVATE ConnectQB5xx PRIVATE CommonH)
add_compile_definitions(SERVEREXSHAREDPLUGIN_HONEY5XX_EXPORTS)

set(PROJECT_INCLUDES
        "${CMAKE_CURRENT_SOURCE_DIR}/"        
	"${CMAKE_CURRENT_SOURCE_DIR}/../../Include"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../../../system/include/rsus"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../common/include"
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${PROJECT_INCLUDES}
    ${COMMON_INCLUDES}
)


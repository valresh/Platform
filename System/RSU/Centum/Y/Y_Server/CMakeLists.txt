cmake_minimum_required (VERSION 3.10) 
set(CMAKE_FOLDER ".RSUs/Centum/Y")

set(PROJECT_NAME Y_Server)
include(../../../../global.cmake)


set(Sources
    "Y_Server_dll.cpp"
)
source_group("Sources" FILES ${Sources})	

set(Impl
	"RsuConnectionImpl.cpp"
	#"RsuConnectionImpl.h"
	"Y_Server.cpp"
	"Y_Server.h"
    "../../../common/include/rsuModel.h"
)
source_group("Impl" FILES ${Impl})	

set(SaveRest
    "ChangedParams.cpp"
    "Y_Params.cpp"
    "Y_States.cpp"
)
source_group("SaveRest" FILES ${SaveRest})

set(StateSerImpl
    "StateSer.cpp"
    "StateSer.h"
    "../../Include/yISerializer.h"
    "../../Include/yMagicKeys.h"
)
source_group("StateSerImpl" FILES ${StateSerImpl})

set(ALL_FILES
    ${Sources}
    ${Impl}
    ${SaveRest}
    ${StateSerImpl}
)
add_library(${PROJECT_NAME} SHARED ${ALL_FILES})

add_compile_definitions(Y_SERVER_EXPORTS)

target_link_directories(${PROJECT_NAME} PUBLIC
    "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}"
)
#add_dependencies (${PROJECT_NAME} )

target_link_libraries(${PROJECT_NAME} Utils RsuX Connect ConnectY)

set(PROJECT_INCLUDES
    "${CMAKE_CURRENT_SOURCE_DIR}/"        
	"${CMAKE_CURRENT_SOURCE_DIR}/../../Include"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../../../system/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../common/include"
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${PROJECT_INCLUDES}
    ${COMMON_INCLUDES}
)


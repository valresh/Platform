cmake_minimum_required (VERSION 3.10) 
set(CMAKE_FOLDER ".RSUs/Centum/Y")

set(PROJECT_NAME Y_Client)
include(../../../../global.cmake)

#set(Headers
    #"../../../common/include/rsuSharedClient.h"
#)
#source_group("Headers" FILES ${Headers})

set(Sources
    "Y_Client_dll.cpp"
)
source_group("Sources" FILES ${Sources})	

set(Impl
	"ACSsupport.cpp"
	#"RsuConnectionImpl.h"
	"Y_RemoteClient.cpp"
	"Y_RemoteClient.h"
)
source_group("Impl" FILES ${Impl})	

set(ALL_FILES
#    ${Headers}
    ${Sources}
    ${Impl}
)
add_library(${PROJECT_NAME} SHARED ${ALL_FILES})

add_compile_definitions(Y_CLIENT_EXPORTS)

target_link_directories(${PROJECT_NAME} PUBLIC
    "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}"
)
#add_dependencies (${PROJECT_NAME} )

target_link_libraries(${PROJECT_NAME} Utils RsuX Connect ConnectY CommonRSUs)

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


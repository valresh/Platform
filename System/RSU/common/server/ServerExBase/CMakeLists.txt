set(PROJECT_NAME ServerExBase)
set(CMAKE_FOLDER ".RSUs/common/server")

include(../../../../global.cmake)

set(RootFiles
    CommandHandler.h
    CommandHandler.cpp
)

set(Headers	
	"TrendFile.h"
	"SMessage.h"
	"ServerThreadBase.h"
	"ServerExBase.h"
	"MessageList.h"
	Serializer.h
	Types.h
)
source_group("Headers" FILES ${Headers})

set(Sources
	"MessageList.cpp"
	"MnemoToModel.cpp"
	"PluginsSup.cpp"
	"SMessage.cpp"
	"TrendFile.cpp"
	Serializer.cpp
)
source_group("Sources" FILES ${Sources})

set(ALL_FILES
	${Headers}
	${Sources}
	${RootFiles}
	"../../../../utf8.manifest"
)

add_compile_definitions(
	SERVEREXBASE_EXPORTS
)

add_library (${PROJECT_NAME} SHARED ${ALL_FILES} )

set(COMMON_INCLUDES
    "${CMAKE_CURRENT_SOURCE_DIR}/"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../../system/include/"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../../system/include/"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../include/"
)

set(PROJECT_INCLUDES
        "${CMAKE_CURRENT_SOURCE_DIR}/"        
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${PROJECT_INCLUDES}
    ${COMMON_INCLUDES}
)
target_link_libraries(${PROJECT_NAME} PUBLIC Utils RsuX CommonRSUs)


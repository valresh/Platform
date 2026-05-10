cmake_minimum_required (VERSION 3.10) 
set(CMAKE_FOLDER ".RSUs/HoneyWell5xx/DCUs")

set(PROJECT_NAME QuickBuilderH)
include(../../../../global.cmake)

set(Headers
	"CalcVar.h"
	"ParmVarInfo.h"
	"QB_Class.h"
	"QbBase.h"
	"QBParamInfo.h"
	"QBPARM_INIT.h"
	"QbRsuX.h"
	"QbTranslator.h"
	"QuickBuilder.h"
	"ScriptHandle.h"
	"stateKeys.h"
	"Trans.h"
)
source_group("Headers" FILES ${Headers})

set(Sources	
	"AnalizeTok.cpp"
	"CalcVar.cpp"
	"Func.cpp"
	"GetTokens.cpp"
	"ParmVarInfo.cpp"
	"Prior.cpp"
	"QB_ANALOG.cpp"
	"QB_STATUS.cpp"
	"QbBase.cpp"
	"QBParamInfo.cpp"
	"QbRsuX.cpp"
	"QbTranslator.cpp"
	"QuickBuilder.cpp"
	"QuickBuilder_Calc.cpp"
	"QuickBuilder_Init.cpp"
	"QuickBuilder_ReadProject.cpp"
	"QuickBuilder_SaveRestoreProject.cpp"
	"QuickBuilder_State.cpp"
	"QuickBuilderH.cpp"
	"RsuConnectionImpl.cpp"
	"ScriptHandle.cpp"
	"Var.cpp"
	"QB_PSA.cpp"
)
source_group("Sources" FILES ${Sources})	

set(ALL_FILES
    ${Headers}
    ${Sources}
)
add_library(${PROJECT_NAME} SHARED ${ALL_FILES})

add_compile_definitions(QUICKBUILDERH5XX_EXPORTS)

target_link_directories(${PROJECT_NAME} PUBLIC
    "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}"
)
add_dependencies (${PROJECT_NAME} MemoryServiceHCD)

target_link_libraries(${PROJECT_NAME} CommonRSUs crosslib RsuX UtilsQB5xx ConnectQB5xx TinyXML2 MemoryServiceQB5xx MemoryServiceHCD ConnectSM5xx)

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


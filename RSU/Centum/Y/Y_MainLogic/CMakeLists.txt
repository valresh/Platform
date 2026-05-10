cmake_minimum_required (VERSION 3.10) 
set(CMAKE_FOLDER ".RSUs/Centum/Y")

set(PROJECT_NAME Y_MainLogic)
include(../../../../global.cmake)

#set(Headers
    #"../../../common/include/rsuSharedClient.h"
#)
#source_group("Headers" FILES ${Headers})

set(Model
    "../../Include/_magicKeysCentum.h"
    "../../Include/_structmacros.h"
    "FindObj.cpp"
    "ManualCreates.cpp"
    "ReadBCVO.cpp"
    "readproject.cpp"
    "ReadProjectEx.cpp"
    "readscs_o.cpp"
    "Utils.cpp"
    "Y_Main_Calc.cpp"
    "Y_Main_Creators.cpp"
    "Y_Main_FormCsvRecord.cpp"
    "Y_Main_SavRestPrj.cpp"
    "Y_Main_Step0.cpp"
    "Y_Main_Step1.cpp"
    "Y_MainLogic.cpp"
    "Y_MainLogic.h"
)
source_group("Model" FILES ${Model})	

set(ObjConnection
	"ObjConnection.cpp"
	"ObjConnection.h"
)
source_group("ObjConnection" FILES ${ObjConnection})	

set(...
	"Y_Main_dll.cpp"
)
source_group("..." FILES ${...})	


set(ALL_FILES
    ${Model}
    ${ObjConnection}
    ${...}
)
add_library(${PROJECT_NAME} SHARED ${ALL_FILES})

add_compile_definitions(Y_MAINLOGIC_EXPORTS)

target_link_directories(${PROJECT_NAME} PUBLIC
    "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}"
)
#add_dependencies (${PROJECT_NAME} )

target_link_libraries(${PROJECT_NAME} Utils CommonY MemoryServiceY CommonRSUs)

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


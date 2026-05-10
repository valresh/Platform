cmake_minimum_required (VERSION 3.10) 
set(CMAKE_FOLDER ".RSUs/HoneyWell5xx/DCUs")

set(PROJECT_NAME SafetyManagerH5xx_Server)
include(../../../../global.cmake)

set(Headers
	"DbfFiles.h"
	"FscBase.h"
	"FscFile.h"
	"FscFiles.h"
	"FscList.h"
	"FscTypes.h"
	"Get2Buffer.h"
	"IFscStorage.h"
	"IStateSer.h"
	"SafetyManager.h"
	"SM_Server.h"
	"stateKeys.h"
	"StateSer.h"
	"FscDeclEnum.hpp"
	"FscEnum.hpp"
	"ISMsPool.h"
	"SMsPool.h"
)
source_group("Headers" FILES ${Headers})

set(Sources	
	"DbfFiles.cpp"
	"Fsc1C.cpp"
	"Fsc4B.cpp"
	"FscBase.cpp"
	"FscFile.cpp"
	"FscFiles.cpp"
	"FscList.cpp"
	"Get2Buffer.cpp"
	"Load.cpp"
	"ReadProject.cpp"
	"RestoryData.cpp"
	"RsuConnectionImpl.cpp"
	"SafetyManager.cpp"
	"SafetyManager_ACS.cpp"
	"SafetyManager_Calc.cpp"
	"SafetyManagerH5xx_Server_dll.cpp"
	"SafetyManagerIimpl.cpp"
	"SM_Server.cpp"
	"SM_States.cpp"
	"StateSer.cpp"
	"SMsPool.cpp"
)
source_group("Sources" FILES ${Sources})	
set(FSC	
	"FSC/Fsc01.cpp"
	"FSC/Fsc02.cpp"
	"FSC/Fsc03.cpp"
	"FSC/Fsc04.cpp"
	"FSC/Fsc05.cpp"
	"FSC/Fsc06.cpp"
	"FSC/Fsc08.cpp"
	"FSC/Fsc09.cpp"
	"FSC/Fsc0A.cpp"
	"FSC/Fsc0B.cpp"
	"FSC/Fsc0C.cpp"
	"FSC/Fsc0D.cpp"
	"FSC/Fsc0E.cpp"
	"FSC/Fsc0F.cpp"
	"FSC/Fsc10.cpp"
	"FSC/Fsc12.cpp"
	"FSC/Fsc13.cpp"
	"FSC/Fsc15.cpp"
	"FSC/Fsc16.cpp"
	"FSC/Fsc17.cpp"
	"FSC/Fsc18.cpp"
	"FSC/Fsc19.cpp"
	"FSC/Fsc1A.cpp"
	"FSC/Fsc1D.cpp"
	"FSC/Fsc1E.cpp"
	"FSC/Fsc1F.cpp"
	"FSC/Fsc20.cpp"
	"FSC/Fsc21.cpp"
	"FSC/Fsc22.cpp"
	"FSC/Fsc23.cpp"
	"FSC/Fsc24.cpp"
	"FSC/Fsc25.cpp"
	"FSC/Fsc26.cpp"
	"FSC/Fsc27.cpp"
	"FSC/Fsc28.cpp"
	"FSC/Fsc29.cpp"
	"FSC/Fsc2A.cpp"
	"FSC/Fsc2B.cpp"
	"FSC/Fsc2C.cpp"
	"FSC/Fsc2D.cpp"
	"FSC/Fsc2E.cpp"
	"FSC/Fsc2F.cpp"
	"FSC/Fsc30.cpp"
	"FSC/Fsc31.cpp"
	"FSC/Fsc32.cpp"
	"FSC/Fsc33.cpp"
	"FSC/Fsc34.cpp"
	"FSC/Fsc35.cpp"
	"FSC/Fsc36.cpp"
	"FSC/Fsc37.cpp"
	"FSC/Fsc3A.cpp"
	"FSC/Fsc3B.cpp"
	"FSC/Fsc3C.cpp"
	"FSC/Fsc3D.cpp"
	"FSC/Fsc3E.cpp"
	"FSC/Fsc41.cpp"
	"FSC/Fsc42.cpp"
	"FSC/Fsc43.cpp"
	"FSC/Fsc44.cpp"
	"FSC/Fsc45.cpp"
	"FSC/Fsc46.cpp"
	"FSC/Fsc47.cpp"
	"FSC/Fsc48.cpp"
	"FSC/Fsc49.cpp"
	"FSC/Fsc4A.cpp"
	"FSC/FscDo.cpp"
)
source_group("FSC" FILES ${FSC})
set(ALL_FILES
    ${Headers}
    ${Sources}
	${FSC}
)
add_library(${PROJECT_NAME} SHARED ${ALL_FILES})

add_compile_definitions(SAFETYMANAGERH5XX_SERVER_EXPORTS)
add_compile_definitions(RSU_SERVER_LIB_EXPORT)

target_link_directories(${PROJECT_NAME} PUBLIC
    "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}"
)
add_dependencies (${PROJECT_NAME} MemoryServiceHCD)

target_link_libraries(${PROJECT_NAME} Utils UtilsSM5xx CommonRSUs crosslib RsuX ConnectSM5xx StateSerHoneyLocal MemoryServiceSM5xx crc32)
if (WIN32)
target_link_libraries(${PROJECT_NAME} SafetyManagerH5xx_Debug)
endif()

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


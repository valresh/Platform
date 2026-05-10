cmake_minimum_required (VERSION 3.10)
project ("MnemoWnd.C++")
include(../../../../global.cmake)

set(Elements
	"Queue/Button.cpp"
	"QueueGDI/Brush.cpp"
	"QueueGDI/Call.cpp"
	"QueueGDI/Header.cpp"
	"QueueGDI/Font.cpp"
	"QueueGDI/FontScript.cpp"
	"QueueGDI/Draw.cpp"
	"QueueGDI/Ellipse.cpp"
	"QueueGDI/FillRect.cpp"
	"QueueGDI/Frame.cpp"
	"QueueGDI/Goto.cpp"
	"QueueGDI/Include.cpp"
	"QueueGDI/IF.cpp"
	"QueueGDI/Image.cpp"
	"QueueGDI/Pen.cpp"
	"QueueGDI/PresentFE.cpp"
	"QueueGDI/Rect.cpp"
	"QueueGDI/TextColor.cpp"
	"QueueGDI/Word.cpp"
	"QueueGDI/Tele.cpp"
	"Kompressor/kcaedit.cpp"
	"Kompressor/kcarect.cpp"
	"Kompressor/Value.cpp"
	
	"Hardware(Manage)/DataST.cpp"
	"Hardware(Manage)/HsST.cpp"
	"Hardware(Manage)/KlapST.cpp"
	"Hardware(Manage)/ValueST.cpp"
	"Hardware(Manage)/ZadvST.cpp"
	"Hardware(Manage)/PumpST.cpp"

	"Hardware(Show)/DataSH.cpp"
	"Hardware(Show)/LabelD.cpp"
	"Hardware(Show)/KlapSH.cpp"
	"Hardware(Show)/ZadvSH.cpp"
	"Hardware(Show)/SensSH.cpp"
)

set(Sources
    "Parser.cpp"
    "PipeClient.cpp"
	"Sterver.cpp"
	"DrawBeko.cpp"
	"SocketClient.cpp"
	"SetQueue.cpp"
	"SetValue.cpp"
	"UniLogic.cpp"
	"MnemoWnd.cpp"
	"Lang.cpp"
	"QueueGDI.cpp"
	"Api/MnemoSession.cpp"
	"Api/MnemoSession.h"
)

set(Include
	"../../include/EnumQueryTypes.h"
	"../../include/PipeClient.h"
	"../../include/Sterver.h"
	"../../../../system/include/UniBuffer.h"
	"../../include/Queue.h"
	"socketclient.h"
	"Function.hpp"
	"Function.h"
	"Lang.h"
	"squeue.h"
	"UniLogic.h"
	"QueueAPM.h"
)
source_group("Include" FILES ${Include})

set(ALL_FILES ${Sources} ${Include} ${Elements})
add_compile_definitions(QUEUEAPM_EXPORTS)
add_library (${PROJECT_NAME} SHARED ${ALL_FILES})

include_directories(
	"${CMAKE_CURRENT_LIST_DIR}"
	"${CMAKE_CURRENT_LIST_DIR}/../../.."
	"${CMAKE_CURRENT_LIST_DIR}/../../../../system/include"
	"${CMAKE_CURRENT_LIST_DIR}/../../../common/include"
	"${CMAKE_CURRENT_LIST_DIR}/../../../../crosslib"
)

target_link_libraries(${PROJECT_NAME} PUBLIC Utils SoftGDI)
if(NOT UNIX)
	target_link_libraries(${PROJECT_NAME} PUBLIC Ws2_32.lib Utils SoftGDI)
endif()

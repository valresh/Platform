# CMakeList.txt : CMake project for HoneywellData, include source and define
# project specific logic here.
#
cmake_minimum_required(VERSION 3.10)

project (ARM_HWL_500)
include(../../../global.cmake)
set(CMAKE_CXX_STANDARD 20)

#add_subdirectory(Utils)


# # Указываем, что проект использует MFC
# set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
# set(CMAKE_DEBUG_POSTFIX "d")

# # Настройка типа приложения
# add_definitions(-D_AFX_DLL)
# add_definitions(-D_AFXWIN_DLL)

# Поиск и подключение библиотеки FormatSCN
# find_package(FormatSCN REQUIRED)

# Add source to this project's executable.
set(Sources
      "stdafx.cpp"
 #    "ARM.cpp"
 #    "HTML/AnalizSPAN.cpp"
 #    "HTML/AnalizTextArea.cpp"
 #    "HTML/BaseHTML.cpp"
 #    "HTML/BaseHTML.h"
 #    "HTML/Bin.cpp"
 #    "HTML/Bin.h"
 #    "HTML/Dsd.cpp"
 #    "HTML/Dsd.h"
 #    "HTML/Hdx.cpp"
 #    "HTML/Hdx.h"
 #    "HTML/HdxBorder.cpp"
 #    "HTML/HdxButton.cpp"
 #    "HTML/HdxLine.cpp"
 #    "HTML/HdxOval.cpp"
 #    "HTML/HdxPaint.cpp"
 #    "HTML/HdxPolygon.cpp"
 #    "HTML/HdxPolyline.cpp"
 #    "HTML/HdxRect.cpp"
 #    "HTML/HdxRound.cpp"
 #    "HTML/HdxType10.cpp"
 #    "HTML/HdxWedge.cpp"
     # "HTML/HTML.cpp"
     # "HTML/HTML.h"
 #    "HTML/HTML_Path.h"
 #    "HTML/Scripts127.cpp"
 #    "HTML/Scripts127.h"
 #    "HTML/Shape.cpp"
 #    "HTML/Shape.h"
      "ARM.cpp"
      "ARM.h"
      "HTML/Style.cpp"
      "HTML/Style.h"
 #    "HTML/StyleClass.cpp"
 #    "HTML/StyleClass.hpp"
      "HTML/Tag.cpp"
      "HTML/Tag.h"
 #    "HTML/TagA.cpp"
 #    "HTML/TagA.h"
 #    "HTML/TagBODY.cpp"
 #    "HTML/TagBODY.h"
 #    "HTML/TagDetail.cpp"
 #    "HTML/TagDetail.h"
 #      "HTML/TagDIV.cpp"
 #      "HTML/TagDIV.h"
 #    "HTML/TagDIV_AL.cpp"
 #    "HTML/TagIMG.cpp"
 #    "HTML/TagIMG.h"
 #    "HTML/TagINPUT.cpp"
 #    "HTML/TagINPUT.h"
 #    "HTML/TagOBJECT.cpp"
 #    "HTML/TagOBJECT.h"
 #    "HTML/TagSCRIPT.cpp"
 #    "HTML/TagSCRIPT.h"
 #    "HTML/TagSPAN.cpp"
 #    "HTML/TagSPAN.h"
 #    "HTML/TagTextArea.cpp"
 #    "HTML/TagTextArea.h"
 #    "HTML/TagTITLE.cpp"
 #    "HTML/TagTITLE.h"
 #    "HTML/VBScript.cpp"
 #    "HTML/VBScript.h"
 #    "HTML/VBScriptList.cpp"
 #    "HTML/VBScriptRun.cpp"
 #    "HTML/VBScriptScan.cpp"
 # #   "stdafx.cpp"
 #    "HoneyMnemoExtend_500/HoneyMnemoExtend.cpp"
 #    "Function.cpp"
 #    #    "Trend3.cpp"
 #    "MnemoFrm.cpp"
    #   "MnemoStates.cpp"
    "Api.cpp"
    # "ViewMnemo.cpp"
    # "MyEdit.cpp"
    # "AmmFrm.cpp"
    # "Profile.cpp"
)
source_group("Sources" FILES ${Sources})

set(Includes
    "Api.h"
    "R300/InterlockContainer.h"
#     "Profile.h"
#     "Arm.h"
     "StdAfx.h"
#     "Include//HoneyMnemoExtend.h"
#     "Include/ShData.h"
#     "Include/HoneywellType.hpp"
#     "Include/BaseType_.h"
# #    "Include/Queue.h"
#     "XmlHelperPanoram.h"
#     "Function.h"
#     #    "Trend3.h"
#     "MnemoFrm.h"
#     "MnemoStates.h"
#     "ViewMnemo.h"
#     #    "HoneyMnemoExtend.h"
#     "Function.h"
#     "MyEdit.h"
    #"HTML_path.h"
    #"Dsd.h"
    #Bin.h
    # "Tag.h"
    # "Style.h"
    # "Hdx.h"
    # "Function.h"
    #"TagA.h"
    #"TagBODY.h"
    #   "TagDIV.h"
    #"TagDetail.h"
    #"TagIMG.h"
    #"TagINPUT.h"
    #"TagOBJECT.h"
    #"TagSCRIPT.h"
    #"TagSPAN.h"
    #"TagTITLE.h"
    #"TagTextArea.h"
    #"VBScript.h"
    # "Style.h"
    # "Hdx.h"
    # "AmmFrm.h"
#    "ButtonFrm.h"

)

set(Types

     "linuxTypes/CString.h"
    "linuxTypes/cpoint.h"
 #   "linuxTypes/atltypes.h"
 #   "linuxTypes/bitmap.h"

    # "Types/BaseType.hpp"
    # "Types/basetype.h"
    # "Types/real.h"
    # "Helpers/staticHelpers.h"

)

#set(Shared
#    "shared/CommProc.h"
#    "shared/HoneyMnemoExtend.h"
#    "shared/PlatformDefs.h"
#    "shared/UniBuffer.h"
#    "shared/utilsqb.h"
#    "shared/crossstring.h"
#    "shared/cutf.h"
#)


#set(Tags
#    # "tags/TagA.cpp"
#    # "tags/TagA.h"
# "tags/TagBODY.cpp"
# "tags/TagBODY.h"
# "tags/TagDetail.cpp"
# "tags/TagDetail.h"
#    "tags/TagDIV.cpp"
#    "tags/TagDIV.h"
#    "tags/TagDIV_AL.cpp"
# "tags/TagIMG.cpp"
# "tags/TagIMG.h"
# "tags/TagINPUT.cpp"
# "tags/TagINPUT.h"
# "tags/TagOBJECT.cpp"
# "tags/TagOBJECT.h"
# "tags/TagSCRIPT.cpp"
# "tags/TagSCRIPT.h"
# "tags/TagSPAN.cpp"
# "tags/TagSPAN.h"
# "tags/TagTextArea.cpp"
# "tags/TagTextArea.h"
# "tags/TagTITLE.cpp"
# "tags/TagTITLE.h"
#)

#source_group("Tags" FILES ${Shared})

#source_group("Shared" FILES ${Shared})

#source_group("Include" FILES ${Includes})

source_group("Types" FILES ${Types})

#include_directories ("Types")




set(ALL_FILES ${Sources} ${Includes}  ${Types})
#target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} "../" "../RSUs/system/include")
add_compile_definitions(WIN32_LEAN_AND_MEAN)
#add_compile_definitions(NOGDI)
#add_compile_definitions(NODRAWTEXT)
#add_compile_definitions(NOMB)

add_compile_definitions(NOGDICAPMASKS    ) #- CC_*, LC_*, PC_*, CP_*, TC_*, RC_
add_compile_definitions(NOVIRTUALKEYCODES) #- VK_*
add_compile_definitions(NOWINMESSAGES    ) #- WM_*, EM_*, LB_*, CB_*
add_compile_definitions(NOWINSTYLES      ) #- WS_*, CS_*, ES_*, LBS_*, SBS_*, CBS_*
add_compile_definitions(NOSYSMETRICS     ) #- SM_*
add_compile_definitions(NOMENUS          ) #- MF_*
add_compile_definitions(NOICONS          ) #- IDI_*
add_compile_definitions(NOKEYSTATES      ) #- MK_*
add_compile_definitions(NOSYSCOMMANDS    ) #- SC_*
add_compile_definitions(NORASTEROPS      ) #- Binary and Tertiary raster ops
add_compile_definitions(NOSHOWWINDOW     ) #- SW_*
add_compile_definitions(OEMRESOURCE      ) #- OEM Resource values
add_compile_definitions(NOATOM           ) #- Atom Manager routines
add_compile_definitions(NOCLIPBOARD      ) #- Clipboard routines
add_compile_definitions(NOCOLOR          ) #- Screen colors
add_compile_definitions(NOCTLMGR         ) #- Control and Dialog routines
add_compile_definitions(NODRAWTEXT       ) #- DrawText() and DT_*
add_compile_definitions(NOGDI            ) #- All GDI defines and routines
add_compile_definitions(NOKERNEL         ) #- All KERNEL defines and routines
add_compile_definitions(NOUSER           ) #- All USER defines and routines
#add_compile_definitions(NONLS            ) #- All NLS defines and routines
add_compile_definitions(NOMB             ) #- MB_* and MessageBox()
add_compile_definitions(NOMEMMGR         ) #- GMEM_*, LMEM_*, GHND, LHND, associated routines
add_compile_definitions(NOMETAFILE       ) #- typedef METAFILEPICT
add_compile_definitions(NOMINMAX         ) #- Macros min(a,b) and max(a,b)
#add_compile_definitions(NOMSG            ) #- typedef MSG and associated routines
add_compile_definitions(NOOPENFILE       ) #- OpenFile(), OemToAnsi, AnsiToOem, and OF_*
add_compile_definitions(NOSCROLL         ) #- SB_* and scrolling routines
add_compile_definitions(NOSERVICE        ) #- All Service Controller routines, SERVICE_ equates, etc.
add_compile_definitions(NOSOUND          ) #- Sound driver routines
add_compile_definitions(NOTEXTMETRIC     ) #- typedef TEXTMETRIC and associated routines
add_compile_definitions(NOWH             ) #- SetWindowsHook and WH_*
add_compile_definitions(NOWINOFFSETS     ) #- GWL_*, GCL_*, associated routines
add_compile_definitions(NOCOMM           ) #- COMM driver routines
add_compile_definitions(NOKANJI          ) #- Kanji support stuff.
add_compile_definitions(NOHELP           ) #- Help engine interface.
add_compile_definitions(NOPROFILER       ) #- Profiler interface.
add_compile_definitions(NODEFERWINDOWPOS ) #- DeferWindowPos routines
add_compile_definitions(NOMCX            ) #- Modem Configuration Extensions


#add_compile_definitions(NOKERNEL)
#add_compile_definitions(NOMINMAX)
#add_compile_definitions(NOUSER)
#add_compile_definitions(_WINUSER_)
#add_compile_definitions(NOMINMAX)
#add_compile_definitions(__MSVC_BIT_UTILS_HPP)
add_library(${PROJECT_NAME} SHARED ${ALL_FILES} "avaloniaWrappers/Gdiplus.h")


target_link_libraries(${PROJECT_NAME} PUBLIC
    #    afxwin
    # mfc${MFC_VERSION}
    # mfcs${MFC_VERSION}d
    # comctl32
    # rpcrt4
    # oleaut32
    # ole32
    # uuid
    # winmm
    # version
    Utils
    cutf
    crosslib
    shlwapi
    MnemoWnd.C++
    # UtilsH
    # UtilsSM

    #    ${FORMATSCN_LIBRARIES}
)

include_directories(
    "../system/include"
    "common"
    ${CMAKE_CURRENT_LIST_DIR}
    "../RSUs/crosslib"
    "../Include"
)




if (CMAKE_VERSION VERSION_GREATER 3.10)
    set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
endif()




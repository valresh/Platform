cmake_minimum_required (VERSION 3.10)
set(PROJECT_NAME FlexRsuTranslator)
include (../../FlexCommon.cmake)

set(Headers
    "FlexTranslator.h"
    "ITranslatorOwner.h"
    "TransTypes.h"
)
source_group("Headers" FILES ${Headers})

set(Sources
    "AnalizeTok.cpp"
    "FlexRsuTranslator.cpp"
    "FlexTranslator.cpp"
    "GetTokens.cpp"
    "IsFunc.cpp"
    "IsSys.cpp"
    "Logic.cpp"
    "Var.cpp"
)
source_group("Sources" FILES ${Sources})

set(ALL_FILES
    ${Headers}
    ${Sources}
    "../../../../utf8.manifest"
)

add_library(${PROJECT_NAME} SHARED ${ALL_FILES})

add_compile_definitions(FLEXRSUTRANSLATOR_EXPORTS)
target_link_libraries(${PROJECT_NAME} CommonRSUs)

set(PROJECT_INCLUDES
        "${CMAKE_CURRENT_SOURCE_DIR}/"
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${COMMON_INCLUDES}
    ${PROJECT_INCLUDES}
)

message (COMMON_INCLUDES="${COMMON_INCLUDES}")


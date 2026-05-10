include (${CMAKE_CURRENT_LIST_DIR}/../../global.cmake)

#project (FlexRsu)
set(CMAKE_FOLDER ".RSUs/FlexRsu/common")

set(COMMON_INCLUDES
    "${CMAKE_CURRENT_SOURCE_DIR}/"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../include/"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../common/include/"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../../system/include/"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../../crosslib/include/"
)

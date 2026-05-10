cmake_minimum_required (VERSION 3.10)
set(CMAKE_FOLDER ".RSUs/common/server")

set(PROJECT_NAME rsu_server)
include (${CMAKE_CURRENT_LIST_DIR}/../../../../global.cmake)

set(COMMON_INCLUDES
    "${CMAKE_CURRENT_SOURCE_DIR}/"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../include/"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../../system/include/"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../../crosslib/include/"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../../system/source/logging"
)

set(Impl
	"rsu_server.cpp"
    "main.rc"
)
source_group("Impl" FILES ${Impl})

set(ALL_FILES
    ${Impl}
    "../../../../utf8.manifest"
)

if (UNIX)
    SET(CMAKE_CXX_STANDARD 17)

    find_package(GTK2 REQUIRED)
    INCLUDE_DIRECTORIES(${GTK2_INCLUDE_DIRS})
    LINK_DIRECTORIES(${GTK2_INCLUDE_DIRS})

    add_executable (${PROJECT_NAME} ${ALL_FILES} )
elseif(WIN32)
    add_executable (${PROJECT_NAME} ${ALL_FILES} )
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
endif ()

#add_executable (${PROJECT_NAME} ${ALL_FILES} )
target_link_libraries(${PROJECT_NAME} PUBLIC ${GTK2_LIBRARIES} crosslib Utils loggingLib)

set(PROJECT_INCLUDES
        "${CMAKE_CURRENT_SOURCE_DIR}/"        
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${PROJECT_INCLUDES}
    ${COMMON_INCLUDES}
)

add_custom_command(
  TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy 
    ${CMAKE_CURRENT_SOURCE_DIR}/RsuServer.ico 
    $<TARGET_FILE_DIR:${PROJECT_NAME}>)


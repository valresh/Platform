set(CMAKE_FOLDER ".RSUs/common/server")
project(ModelEx)
include(../../../../global.cmake)

set(ALL_FILES 
	res/main.rc 
	main.cpp 
	SimulationControl.h
	../../../../utf8.manifest
	PasswordPolicy.h
    PasswordPolicy.cpp
    MqttConfigurator.h
    MqttConfigurator.cpp
    MqttSettings.h
    ConsoleHelper.h)

if (UNIX)
    #SET(CMAKE_CXX_STANDARD 17)
    find_package(GTK2 REQUIRED)
    INCLUDE_DIRECTORIES(${GTK2_INCLUDE_DIRS})
    LINK_DIRECTORIES(${GTK2_INCLUDE_DIRS})
    set(PLATFORM_LIBRARIES ${GTK2_LIBRARIES})
    add_executable(${PROJECT_NAME} ${ALL_FILES} )

elseif (WIN32)
    set(PLATFORM_LIBRARIES user32 gdi32)
    add_executable(${PROJECT_NAME} ${ALL_FILES} )
    set_target_properties(ModelEx PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
endif ()

target_include_directories(${PROJECT_NAME} PRIVATE
    "../../../../system/source/logging"
    "../../../../system/source/System/modelexserver"
    "../../include"
    "../../../../system/external"
)
target_link_libraries(${PROJECT_NAME} PUBLIC ${PLATFORM_LIBRARIES} cutf loggingLib modelexserver Utils crosslib aes-c)
add_custom_command(
  TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy 
    ${CMAKE_CURRENT_SOURCE_DIR}/res/ModelEx.ico 
    $<TARGET_FILE_DIR:${PROJECT_NAME}>)



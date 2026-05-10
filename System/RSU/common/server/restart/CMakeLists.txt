cmake_minimum_required (VERSION 3.10)
set(CMAKE_FOLDER ".RSUs/common/server")
project(restart)
include(../../../../global.cmake)

set(Sources	
	"restart.cpp"
	"stdafx.cpp"
	"stdafx.h"
)

add_executable(${PROJECT_NAME} WIN32 ${Sources} )

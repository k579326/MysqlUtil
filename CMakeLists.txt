
cmake_minimum_required(VERSION 2.8)

add_definitions(-D_CRT_SECURE_NO_WARNINGS)

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
	foreach(flag_var
		CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
		CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
		string(REGEX REPLACE "/MDd" "/MD" ${flag_var} "${${flag_var}}")	
	endforeach(flag_var)
	foreach(flag_var
		CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE
		CMAKE_C_FLAGS_MINSIZEREL CMAKE_C_FLAGS_RELWITHDEBINFO)
		string(REGEX REPLACE "/MDd" "/MD" ${flag_var} "${${flag_var}}")	
	endforeach(flag_var)

	# mysql header
	
	add_definitions(-DHAVE_STRUCT_TIMESPEC)
	include_directories(third/pthread-win/include)
	include_directories(include)
	include_directories(include/mysql)
	# mysql lib
	link_directories(${CMAKE_SOURCE_DIR}/lib)
	link_directories(${CMAKE_SOURCE_DIR}/src/third/pthread-win/lib)
else ()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
	include_directories(include)
	include_directories(/usr/include/mysql)
	link_directories(/usr/lib64/mysql)
endif ()

include_directories(include)
include_directories(src/utils)
include_directories(src/third/tinyxml)

add_subdirectory(src)
add_subdirectory(test)


cmake_minimum_required(VERSION 3.0)

aux_source_directory(./command COMMANDSRC)

include_directories(core)

add_library(MYSQLUTIL STATIC 
			${COMMANDSRC}
			${COMMONSRC}
			core/sqlcore.cpp
			core/sqlcore.h
			sqloperator.cpp
			)
			
target_link_libraries(MYSQLUTIL tinyxml 
								#pthread
								mysqlclient
								)

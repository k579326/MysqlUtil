

cmake_minimum_required(VERSION 2.8)

add_executable(SQLTOOL main.cpp readDB.h readDB.cpp
			genClass/gen-tableclass.cpp genClass/gen-tableclass.h
			${COMMONSRC})

set_target_properties(SQLTOOL PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")			
target_link_libraries(SQLTOOL mysqlclient tinyxml)

add_custom_command(TARGET SQLTOOL PRE_LINK 
			COMMENT "COMMAND Execute Copy Lib..."
			COMMAND cmake -E make_directory ${CMAKE_SOURCE_DIR}/bin
			COMMAND cmake -E copy_directory ${CMAKE_SOURCE_DIR}/lib/ ${CMAKE_SOURCE_DIR}/bin/
			#COMMAND cmake -E copy ${CMAKE_SOURCE_DIR}/lib/ ${CMAKE_SOURCE_DIR}/bin/
            #COMMAND cmake -E copy $<TARGET_FILE:${APINAME}> $<TARGET_FILE_DIR:${APINAME}>/${ANDROID_ABI}/$<TARGET_FILE_NAME:${APINAME}>
            #objcopy分离debug符号用于发布，带符号用于CrashDump分析
            #COMMAND cmake -E make_directory ${INSTALL_PATH}/${DOE_BUILD_TIME}/${ANDROID_ABI}
            #COMMAND cmake -E copy $<TARGET_FILE:${APINAME}> ${INSTALL_PATH}/${DOE_BUILD_TIME}/RelWithDebug/${ANDROID_ABI}/$<TARGET_FILE_NAME:${APINAME}>
            )
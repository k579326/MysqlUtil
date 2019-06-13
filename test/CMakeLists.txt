

cmake_minimum_required(VERSION 2.8)

file(GLOB TESTSRC ./*.hpp)
add_executable(testSql test.cpp ${TESTSRC})
target_link_libraries(testSql MYSQLUTIL)


add_custom_command(TARGET testSql PRE_LINK
	COMMENT "copy database config file to test dir ..."
	COMMAND cmake -E make_directory $<TARGET_FILE_DIR:testSql>/config
	COMMAND cmake -E copy ${CMAKE_SOURCE_DIR}/lib/config/remote.xml $<TARGET_FILE_DIR:testSql>/config/remote.xml
)

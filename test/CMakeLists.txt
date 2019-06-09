

cmake_minimum_required(VERSION 2.8)

file(GLOB TESTSRC ./*.hpp)
add_executable(testSql test.cpp ${TESTSRC})
target_link_libraries(testSql MYSQLUTIL)



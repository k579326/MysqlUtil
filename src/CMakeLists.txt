

cmake_minimum_required(VERSION 3.0)

file(GLOB COMMONSRC common/*)
file(GLOB POOLSRC pool/*)

include_directories(pool)
include_directories(inc)

add_subdirectory(third/tinyxml)
add_subdirectory(SQLTool)
add_subdirectory(ezSQL)
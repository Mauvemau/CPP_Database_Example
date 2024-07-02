cmake_minimum_required(VERSION 3.25)
project(mysqlg)

file(GLOB_RECURSE SOURCES "Game/src/*.cpp")

set(MYSQL_INCLUDE_DIR /usr/include/mysql)
set(MYSQL_LIBRARIES /usr/lib64/libmariadb.so)

include_directories(Game/src)
include_directories(${MySQL_INCLUDE_DIR})
add_executable(mysqlg ${SOURCES})

target_link_libraries(mysqlg ${MySQL_LIBRARIES})
target_link_libraries(mysqlg mariadb)

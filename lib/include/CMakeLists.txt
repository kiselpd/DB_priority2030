cmake_minimum_required(VERSION 3.16)
project(lib)

file(GLOB JWT_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/jwt)
file(GLOB DATABASE_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/database)
file(GLOB SESSION_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/session)
file(GLOB SERVER_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/server)

set(JWT_INCLUDE_DIRS ${JWT_INCLUDE_DIRS} PARENT_SCOPE)
set(DATABASE_INCLUDE_DIRS ${DATABASE_INCLUDE_DIRS} PARENT_SCOPE)
set(SESSION_INCLUDE_DIRS ${SESSION_INCLUDE_DIRS} PARENT_SCOPE)
set(SERVER_INCLUDE_DIRS ${SERVER_INCLUDE_DIRS} PARENT_SCOPE)


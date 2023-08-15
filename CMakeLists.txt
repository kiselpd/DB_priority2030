cmake_minimum_required(VERSION 2.8)

project(database)		

set(SOURCE_EXE main.cpp)			# Установка переменной со списком исходников

include_directories(database/include)			# Расположение заголовочных файлов

set(SOURCE_FILES
        main.cpp
        database/src/client_acceptor.cpp
        database/src/client_session.cpp
        database/src/connection_pool.cpp
        database/src/connection.cpp
        database/src/db_manager.cpp
        database/src/request.cpp
        )
 
add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} pqxx pthread)
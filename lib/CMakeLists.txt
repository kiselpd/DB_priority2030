project(lib)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(additional_lib/json)
add_subdirectory(additional_lib/cpp-jwt)

add_subdirectory(include)
add_subdirectory(src)

file(GLOB INCLUDE_DIRS
        ${JWT_INCLUDE_DIRS}
        ${DATABASE_INCLUDE_DIRS}
        ${SESSION_INCLUDE_DIRS}
        ${SERVER_INCLUDE_DIRS}
)

file(GLOB SOURCE_FILES
        ${JWT_SOURCE_FILES}
        ${DATABASE_SOURCE_FILES}
        ${SESSION_SOURCE_FILES}
        ${SERVER_SOURCE_FILES}
)

add_library(${PROJECT_NAME} ${SOURCE_FILES})
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME} pqxx pthread nlohmann_json::nlohmann_json cpp-jwt::cpp-jwt config++)

set(CORE_LIBRARIES ${PROJECT_NAME} PARENT_SCOPE)
set(CORE_INCLUDE_DIRS ${INCLUDE_DIRS} PARENT_SCOPE)
cmake_minimum_required(VERSION 3.10)
project(app)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(
    application/Repositories
    application/Usercases
    domain
)

include_directories(install/include)

link_directories(${CMAKE_CURRENT_SOURCE_DIR}/install/lib)

file(GLOB_RECURSE SOURCES 
    "main.cpp"
    "application/*.h" 
    "domain/*.h"
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} hellnah_db)

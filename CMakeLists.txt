cmake_minimum_required(VERSION 3.10)
project(universal_launcher_server)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(
    core
    core/domain
    core/application
    core/infrastructure
    core/infrastructure/install/include
)

file(GLOB_RECURSE SOURCES 
    "core/domain/*.cpp"
    "core/application/*.cpp"
    "core/infrastructure/*.cpp"
    "core/api/*.cpp"
)

add_executable(launcher_server ${SOURCES})

find_package(Threads REQUIRED)
target_link_libraries(launcher_server PRIVATE Threads::Threads)

set_target_properties(launcher_server PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin"
)

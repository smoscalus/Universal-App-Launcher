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
    "main.cpp"
    "core/domain/*.cpp"
    "core/application/*.cpp"
    "core/infrastructure/*.cpp"
    "core/api/*.cpp"
)

add_executable(launcher_server ${SOURCES})

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
pkg_check_modules(WEBKIT REQUIRED webkit2gtk-4.0)

target_include_directories(launcher_server PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party/webview_sdk
    ${GTK3_INCLUDE_DIRS}
    ${WEBKIT_INCLUDE_DIRS}
)

set(HELLNAH_LIB_PATH "${CMAKE_CURRENT_SOURCE_DIR}/core/infrastructure/install/lib/libhellnah_db.a")

find_package(Threads REQUIRED)

target_link_libraries(launcher_server PRIVATE
    Threads::Threads
    ${HELLNAH_LIB_PATH}
    ${GTK3_LIBRARIES}
    ${WEBKIT_LIBRARIES}
    dl
)

set_target_properties(launcher_server PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin"
)

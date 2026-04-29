cmake_minimum_required(VERSION 3.10)
project(universal_launcher_wasm)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(
    core/domain
    core/application
    core/infrastructure
    install/include
)

file(GLOB_RECURSE SOURCES 
    "core/domain/*.cpp"
    "core/application/*.cpp"
    "core/infrastructure/*.cpp"
    "core/api/*.cpp"
)

if(EMSCRIPTEN)
    add_executable(launcher ${SOURCES})

    target_link_options(launcher PRIVATE
        "-sWASM=1"
        "-sMODULARIZE=1"
        "-sEXPORT_NAME='createLauncherModule'"
        "--bind"
        "-sALLOW_MEMORY_GROWTH=1"
        "-sFS_DEBUG=1"
    )

    set_target_properties(launcher PROPERTIES 
        OUTPUT_NAME "launcher"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/client/wasm_lib"
    )
else()
    add_executable(launcher_native ${SOURCES} main.cpp)
    target_link_libraries(launcher_native hellnah_db)
endif()
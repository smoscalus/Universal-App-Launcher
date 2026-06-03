cmake_minimum_required(VERSION 3.10)
project(universal_launcher_server)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Шляхи для інклюдів (залишаємо як є)
include_directories(
    core
    core/domain
    core/application
    core/infrastructure
    core/infrastructure/install/include
)

# Знаходимо всі .cpp файли твого сервера (контролери, сервіси тощо)
file(GLOB_RECURSE SOURCES 
    "main.cpp"
    "core/domain/*.cpp"
    "core/application/*.cpp"
    "core/infrastructure/*.cpp"
    "core/api/*.cpp"
)

add_executable(launcher_server ${SOURCES})

# --- ПІДКЛЮЧЕННЯ СИСТЕМНИХ БІБЛІОТЕК ДЛЯ WEBVIEW (LINUX) ---
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
pkg_check_modules(WEBKIT REQUIRED webkit2gtk-4.0)

# Указываем компилятору, где искать заголовочные файлы Webview, GTK и WebKit
target_include_directories(launcher_server PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party/webview_sdk
    ${GTK3_INCLUDE_DIRS}
    ${WEBKIT_INCLUDE_DIRS}
)
# --- ---

# --- ПІДКЛЮЧЕННЯ БІБЛІОТЕКИ HELL NAH ---

# Вказуємо шлях до файлу .a на основі твого скриншоту
set(HELLNAH_LIB_PATH "${CMAKE_CURRENT_SOURCE_DIR}/core/infrastructure/install/lib/libhellnah_db.a")

find_package(Threads REQUIRED)

# Додаємо Threads, системні бібліотеки Linux та нашу статичну бібліотеку до лінкера
target_link_libraries(launcher_server PRIVATE 
    Threads::Threads
    ${HELLNAH_LIB_PATH}
    ${GTK3_LIBRARIES}     # Линкуем GTK
    ${WEBKIT_LIBRARIES}   # Линкуем WebKit
    dl                     # Потрібно для Crow/Linux
)

# --- ---

set_target_properties(launcher_server PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin"
)
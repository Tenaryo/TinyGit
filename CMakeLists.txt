cmake_minimum_required(VERSION 3.21)

project(
    tiny-git
    VERSION 1.0.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(tinygit_core INTERFACE)
target_include_directories(tinygit_core INTERFACE ${CMAKE_SOURCE_DIR}/src)
target_compile_features(tinygit_core INTERFACE cxx_std_23)
target_compile_options(
    tinygit_core
    INTERFACE -Wall
              -Wextra
              -Wpedantic
              -Werror
              -Wshadow
              -Wconversion
              $<$<CONFIG:Debug>:-g
              -O0>
              $<$<CONFIG:Release>:-O3>
)

find_package(OpenSSL REQUIRED)
find_package(ZLIB REQUIRED)

file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS src/*.cpp)

add_executable(tiny-git ${SOURCE_FILES})
target_link_libraries(tiny-git PRIVATE tinygit_core OpenSSL::Crypto ZLIB::ZLIB)

option(ENABLE_SANITIZERS "Enable Address and Undefined Behavior Sanitizers" OFF)

if (ENABLE_SANITIZERS)
    target_compile_options(tinygit_core INTERFACE -fsanitize=address,undefined -fno-omit-frame-pointer)
    target_link_options(tinygit_core INTERFACE -fsanitize=address,undefined)
endif ()

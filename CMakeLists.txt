cmake_minimum_required(VERSION 3.13)

project(mini_git)

set(CMAKE_CXX_STANDARD 23)

file(GLOB_RECURSE SOURCE_FILES src/*.cpp)
file(GLOB_RECURSE HEADER_FILES include/*.hpp)

find_package(OpenSSL REQUIRED)
find_package(ZLIB REQUIRED)

add_executable(git ${SOURCE_FILES} ${HEADER_FILES})

target_include_directories(git PRIVATE ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(git PRIVATE OpenSSL::Crypto)
target_link_libraries(git PRIVATE ZLIB::ZLIB)

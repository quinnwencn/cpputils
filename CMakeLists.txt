cmake_minimum_required(VERSION 3.15)
project(cpputils LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include("${CMAKE_CURRENT_BINARY_DIR}/generators/conan_toolchain.cmake")

add_subdirectory(src)

add_library(cpputils SHARED
    $<TARGET_OBJECTS:string_obj>
    $<TARGET_OBJECTS:fileops_obj>
)

target_include_directories(cpputils PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)

install(TARGETS cpputils 
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib 
    RUNTIME DESTINATION bin
)

install(DIRECTORY include/cpputils DESTINATION include FILES_MATCHING PATTERN "*.h")
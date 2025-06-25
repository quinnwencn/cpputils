cmake_minimum_required(VERSION 3.15)
project(cpputils LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(src)

add_library(${PROJECT_NAME} SHARED
    $<TARGET_OBJECTS:string_obj>
    $<TARGET_OBJECTS:fileops_obj>
)

target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)

install(TARGETS ${PROJECT_NAME} 
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib 
    RUNTIME DESTINATION bin
)

install(DIRECTORY include/cpputils DESTINATION include FILES_MATCHING PATTERN "*.h")
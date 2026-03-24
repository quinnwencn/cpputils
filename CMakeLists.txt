cmake_minimum_required(VERSION 3.15)
project(cpputils LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB_RECURSE SRCS CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/src/*.cc)

add_library(${PROJECT_NAME} SHARED ${SRCS})

target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)

install(TARGETS ${PROJECT_NAME} 
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib 
    RUNTIME DESTINATION bin
)

install(DIRECTORY include/cpputils DESTINATION include FILES_MATCHING PATTERN "*.h")

option(BUILD_TESTING "Build unit tests" OFF)
if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(test)
endif()
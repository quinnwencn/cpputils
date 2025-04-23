cmake_minimum_required(VERSION 3.15)
project(cpputils_test LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE TEST_SOURCES CMAKE_CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc"
    "${CMAKE_CURRENT_SOURCE_DsIR}/src/*.h"
)

find_package(GTest REQUIRED)
find_package(cpputils REQUIRED)

add_executable(${PROJECT_NAME} ${TEST_SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE 
    "${CMAKE_SOURCE_DIR}/include"
    ${GTest_INCLUDE_DIRS}
    ${cpputils_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE
    ${GTest_LIBRARIES}
    cpputils::cpputils
)

enable_testing()
add_test(NAME cpputils-test COMMAND ${PROJECT_NAME})

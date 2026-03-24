find_package(GTest REQUIRED)

file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cc")

add_executable(cpputils_unit_tests ${TEST_SOURCES})

target_link_libraries(cpputils_unit_tests PRIVATE
    cpputils
    GTest::gtest
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(cpputils_unit_tests)

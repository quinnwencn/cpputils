file(GLOB_RECURSE STRING_SRCS CMAKE_CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/*.cc)
add_library(string_obj OBJECT ${STRING_SRCS})
target_include_directories(string_obj PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)
set_target_properties(string_obj PROPERTIES
    POSITION_INDEPENDENT_CODE ON
)
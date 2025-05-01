file(GLOB_RECURSE FILEOPS_SRCS CMAKE_CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/*.cc)
add_library(fileops_obj OBJECT ${FILEOPS_SRCS})
target_include_directories(fileops_obj PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)
set_target_properties(fileops_obj PROPERTIES
    POSITION_INDEPENDENT_CODE ON
)
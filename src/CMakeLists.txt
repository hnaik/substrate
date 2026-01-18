file(GLOB SBS_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
add_library(sbs_internal ${SBS_SRCS})
target_include_directories(sbs_internal PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")


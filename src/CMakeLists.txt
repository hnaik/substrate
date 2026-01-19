file(GLOB SBS_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
add_library(substrate_internal ${SBS_SRCS})
target_include_directories(
    substrate_internal PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_BINARY_DIR}/generated"
)
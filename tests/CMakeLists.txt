file(GLOB TEST_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
add_executable(all_tests ${TEST_SRCS})
target_link_libraries(all_tests PRIVATE 
    substrate
    substrate_sbe
    Catch2::Catch2WithMain)
copy_program(all_tests)

include(CTest)
include(Catch)
catch_discover_tests(all_tests)

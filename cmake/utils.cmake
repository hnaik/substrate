function(copy_program name)
    add_custom_command(
        TARGET ${name}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ARGS "$<TARGET_FILE:${name}>" "${CMAKE_BINARY_DIR}/"
        COMMENT "Copying ${name} to ${CMAKE_BINARY_DIR}"
    )
endfunction()

find_package(spdlog REQUIRED)

function(add_app name)
    add_executable(${name} ${name}/main.cpp ${ARGN})
    target_link_libraries(${name} substrate substrate_sbe spdlog::spdlog_header_only)
    copy_program(${name})
endfunction()

add_compile_options(
        -Wall                    # Enable most warnings
        -Wextra                  # Extra warnings
        -Wpedantic               # Strict ISO C++ compliance
        -Werror                  # Treat warnings as errors
        -Wcast-align             # Warn on pointer casts that increase alignment
        -Wcast-qual              # Warn on casts that remove qualifiers
        -Wconversion             # Warn on implicit conversions that may alter values
        -Wdouble-promotion       # Warn on implicit float to double promotion
        -Wformat=2               # Extra format string checking
        -Wnon-virtual-dtor       # Warn on non-virtual destructors in classes with virtual functions
        -Wold-style-cast         # Warn on C-style casts
        -Woverloaded-virtual     # Warn on hidden virtual functions
        -Wshadow                 # Warn on variable shadowing
        -Wsign-conversion        # Warn on sign conversions
        -Wunused                 # Warn on unused entities
        -Wnull-dereference       # Warn on potential null dereferences
        -Wduplicated-cond        # Warn on duplicated if-else conditions (GCC)
        -Wduplicated-branches    # Warn on duplicated branches (GCC)
        -Wlogical-op             # Warn on suspicious logical operations (GCC)
)

find_package(Git QUIET)
if(GIT_FOUND)
    execute_process(
        COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_COMMIT_HASH
        ERROR_QUIET
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    message(STATUS "Git commit hash: ${GIT_COMMIT_HASH}")
else()
    set(GIT_COMMIT_HASH "unknown")
    message(STATUS "Git not found, using 'unknown' for version")
endif()

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/src/version.h.in"
    "${CMAKE_CURRENT_BINARY_DIR}/generated/substrate/version.h"
)

# target_include_directories()
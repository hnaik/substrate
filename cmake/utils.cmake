function(copy_program name)
    add_custom_command(
        TARGET ${name}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ARGS "$<TARGET_FILE:${name}>" "${CMAKE_BINARY_DIR}/"
        COMMENT "Copying ${name} to ${CMAKE_BINARY_DIR}"
    )
endfunction()

function(add_app name)
    add_executable(${name} ${name}/main.cpp ${ARGN})
    target_link_libraries(${name} substrate)
    copy_program(${name})
endfunction()

# add_compile_options(
#         -Wall                    # Enable most warnings
#         -Wextra                  # Extra warnings
#         -Wpedantic               # Strict ISO C++ compliance
#         -Werror                  # Treat warnings as errors
#         -Wcast-align             # Warn on pointer casts that increase alignment
#         -Wcast-qual              # Warn on casts that remove qualifiers
#         -Wconversion             # Warn on implicit conversions that may alter values
#         -Wdouble-promotion       # Warn on implicit float to double promotion
#         -Wformat=2               # Extra format string checking
#         -Wnon-virtual-dtor       # Warn on non-virtual destructors in classes with virtual functions
#         -Wold-style-cast         # Warn on C-style casts
#         -Woverloaded-virtual     # Warn on hidden virtual functions
#         -Wshadow                 # Warn on variable shadowing
#         -Wsign-conversion        # Warn on sign conversions
#         -Wunused                 # Warn on unused entities
#         -Wnull-dereference       # Warn on potential null dereferences
#         -Wduplicated-cond        # Warn on duplicated if-else conditions (GCC)
#         -Wduplicated-branches    # Warn on duplicated branches (GCC)
#         -Wlogical-op             # Warn on suspicious logical operations (GCC)
# )

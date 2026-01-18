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
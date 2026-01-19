# # Download the pre-built jar directly from Maven
# FetchContent_Declare(
#     sbe_jar
#     URL https://repo1.maven.org/maven2/uk/co/real-logic/sbe-all/1.37.1/sbe-all-1.37.1.jar
#     DOWNLOAD_NO_EXTRACT TRUE
# )
# FetchContent_MakeAvailable(sbe_jar)

# # Set the jar path
# set(SBE_JAR ${sbe_jar_SOURCE_DIR}/sbe-all-1.37.1.jar)
# message(STATUS "SBE jar location: ${SBE_JAR}")

# # Verify it exists
# if(NOT EXISTS ${SBE_JAR})
#     message(FATAL_ERROR "SBE jar not found at ${SBE_JAR}")
# endif()

# set(SBE_TOOL java -jar ${SBE_JAR})
find_package(Java REQUIRED COMPONENTS Runtime)

# Download pre-built jar
set(SBE_VERSION "1.37.1")
set(SBE_JAR_URL "https://repo1.maven.org/maven2/uk/co/real-logic/sbe-all/${SBE_VERSION}/sbe-all-${SBE_VERSION}.jar")
set(SBE_JAR_PATH "${CMAKE_BINARY_DIR}/sbe/sbe-all-${SBE_VERSION}.jar")

if(NOT EXISTS ${SBE_JAR_PATH})
    message(STATUS "Downloading SBE jar from ${SBE_JAR_URL}")
    file(DOWNLOAD 
        ${SBE_JAR_URL}
        ${SBE_JAR_PATH}
        SHOW_PROGRESS
        STATUS download_status
    )
    list(GET download_status 0 status_code)
    if(NOT status_code EQUAL 0)
        message(FATAL_ERROR "Failed to download SBE jar")
    endif()
endif()

set(SBE_JAR ${SBE_JAR_PATH})
set(SBE_TOOL java -jar ${SBE_JAR})
message(STATUS "SBE tool: ${SBE_TOOL}")

# Verify Java is available
find_package(Java REQUIRED COMPONENTS Runtime)


FetchContent_Declare(
    sbe
    GIT_REPOSITORY https://github.com/aeron-io/simple-binary-encoding.git
    GIT_TAG 1.37.1
)

FetchContent_MakeAvailable(sbe)

# Find the SBE tool
find_program(SBE_TOOL
    NAMES sbe-tool
    PATHS ${CMAKE_BINARY_DIR}/external/sbe/install/bin
)

if(NOT SBE_TOOL)
    # Fallback to jar
    find_file(SBE_JAR
        NAMES sbe-all-1.30.0.jar sbe-all.jar
        PATHS ${CMAKE_BINARY_DIR}/external/sbe/install/bin
    )
    if(SBE_JAR)
        set(SBE_TOOL java -jar ${SBE_JAR})
    else()
        message(FATAL_ERROR "SBE tool not found")
    endif()
endif()

set(SBE_JAR ${SBE_JAR_PATH})

find_package(Java REQUIRED COMPONENTS Runtime)

set(SBE_JAR ${SBE_JAR_PATH})
message(STATUS "SBE jar: ${SBE_JAR}")
message(STATUS "Java executable: ${Java_JAVA_EXECUTABLE}")

# Generation function with correct argument order
function(add_sbe_schema schema_file)
    get_filename_component(schema_name ${schema_file} NAME_WE)
    set(output_dir ${CMAKE_BINARY_DIR}/generated/${schema_name})
    
    message(STATUS "SBE generation configured for: ${schema_file}")
    message(STATUS "Output directory will be: ${output_dir}")
    
    add_custom_command(
        OUTPUT ${output_dir}/.timestamp
        COMMAND ${CMAKE_COMMAND} -E make_directory ${output_dir}
        COMMAND ${Java_JAVA_EXECUTABLE}
            -Dsbe.xinclude.aware=true
            -Dsbe.output.dir=${output_dir}
            -Dsbe.target.language=Cpp
            -Dsbe.cpp.namespaces.collapse=true
            -Dsbe.cpp11.enabled=true
            -Dsbe.generate.stubs=false
            -Dsbe.cpp.template.dir=${output_dir}/templates
            -jar ${SBE_JAR}
            ${schema_file}
        COMMAND ${CMAKE_COMMAND} -E touch ${output_dir}/.timestamp
        DEPENDS ${schema_file}
        COMMENT "Generating SBE C++ headers from ${schema_name}"
        VERBATIM
    )
    
    add_custom_target(${schema_name}_sbe_gen ALL
        DEPENDS ${output_dir}/.timestamp
    )

    add_library(${schema_name}_sbe INTERFACE)
    add_dependencies(${schema_name}_sbe ${schema_name}_sbe_gen)

    # Mark as SYSTEM to suppress warnings
    target_include_directories(${schema_name}_sbe
        SYSTEM INTERFACE
        ${CMAKE_BINARY_DIR}/generated
    )
    
    set(SBE_${schema_name}_INCLUDE_DIR ${output_dir} PARENT_SCOPE)
endfunction()

cmake_minimum_required(VERSION 4.2.1)
project(substrate VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)

include(FetchContent)

find_package(Catch2 3 REQUIRED)

include(cmake/includes.cmake)

add_subdirectory(src)
add_subdirectory(include)
add_subdirectory(apps)
add_subdirectory(tests)

# find_program(SBE_TOOL)
add_sbe_schema(${CMAKE_CURRENT_SOURCE_DIR}/protocol/sbe/substrate.xml)

cmake_minimum_required(VERSION 3.30)
project(substrate VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Catch2 3 REQUIRED)

include(utils.cmake)

add_subdirectory(src)
add_subdirectory(include)
add_subdirectory(apps)
add_subdirectory(tests)
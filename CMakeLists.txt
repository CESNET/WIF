cmake_minimum_required(VERSION 3.12)

set(VERSION_MAJOR 2)
set(VERSION_MINOR 0)
set(VERSION_PATCH 1)
set(VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})

project(wif VERSION ${VERSION})

include(cmake/build_type.cmake)
include(cmake/installation.cmake)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)

option(BUILD_WITH_UNIREC "Enable additional libwif features, which depend on libunirec(++)" OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Wall -Wextra")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -Werror")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -ggdb3")

include(cmake/dependencies.cmake)

add_subdirectory(src)
add_subdirectory(pkg)

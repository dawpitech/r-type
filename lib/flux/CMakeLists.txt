cmake_minimum_required(VERSION 3.10)

set(CMAKE_GENERATOR
    "Unix Makefiles"
    CACHE STRING "generator"
)
set(CMAKE_CXX_STANDARD 17)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

project(flux
    VERSION 1.0.0
    DESCRIPTION "Fast, Layered Universal eXecution"
    LANGUAGES CXX
)

add_library(flux INTERFACE)
target_include_directories(flux INTERFACE include)

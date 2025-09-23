cmake_minimum_required(VERSION 3.10)

project(R-Type
        VERSION 1.0.0
        DESCRIPTION "Touhou but with aliens"
        LANGUAGES CXX
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_GENERATOR
        "Unix Makefiles"
        CACHE STRING "generator"
)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -g3 -O0")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g3 -O0")
endif()

add_subdirectory(lib/flux)

add_subdirectory(client)
add_subdirectory(server)

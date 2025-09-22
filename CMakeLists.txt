cmake_minimum_required(VERSION 3.10)

set(CMAKE_GENERATOR
        "Unix Makefiles"
        CACHE STRING "generator"
)

project(R-Type
        VERSION 1.0.0
        DESCRIPTION "Touhou but with aliens"
        LANGUAGES CXX
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -g3 -O0")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g3 -O0")
endif()

add_subdirectory(lib/flux)

file(GLOB_RECURSE src "src/*.cpp")
add_executable(r-type ${src})

target_link_libraries(r-type PRIVATE flux)
set_target_properties(flux
        PROPERTIES PREFIX "${CMAKE_SOURCE_DIR}/"
)

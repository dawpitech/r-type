cmake_minimum_required(VERSION 3.10)
project(r-type_SDL2 LANGUAGES CXX)

include(FetchContent)

FetchContent_Declare(
    SDL2
    GIT_REPOSITORY https://github.com/libsdl-org/SDL
    GIT_TAG release-3.2.22
)
FetchContent_MakeAvailable(SDL2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES src/main.cpp)

add_executable(r-type ${SOURCES})

# Link SDL2 libraries
target_link_libraries(r-type SDL2 SDL2main)

# Set output directory
set_target_properties(r-type PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

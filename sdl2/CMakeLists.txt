cmake_minimum_required(VERSION 3.10)
project(r-type_SDL2 LANGUAGES CXX)

find_package(PkgConfig)
pkg_check_modules(SDL2 sdl2)
pkg_check_modules(SDL2_IMAGE SDL2_image)
pkg_check_modules(SDL2_TTF SDL2_ttf)

if (NOT SDL2_FOUND OR NOT SDL2_IMAGE_FOUND OR NOT SDL2_TTF_FOUND)
include(FetchContent)

FetchContent_Declare(
    SDL2
    GIT_REPOSITORY https://github.com/libsdl-org/SDL
    GIT_TAG release-3.2.22
)
FetchContent_MakeAvailable(SDL2)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES src/main.cpp)

add_executable(r-type ${SOURCES})

# Link SDL2 libraries
target_link_libraries(r-type SDL2 SDL2main)

# Set output directory
set_target_properties(r-type PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

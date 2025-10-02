cmake_minimum_required(VERSION 3.10)

include(FetchContent)

FetchContent_Declare(
    SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL
    GIT_TAG release-3.2.22
)
FetchContent_MakeAvailable(SDL3)

FetchContent_Declare(
    SDL3_ttf
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf
    GIT_TAG release-3.2.0
)
FetchContent_MakeAvailable(SDL3_ttf)

FetchContent_Declare(
    SDL3_image
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_image
    GIT_TAG release-3.2.4
)
FetchContent_MakeAvailable(SDL3_image)

file(GLOB_RECURSE src "src/*.cpp")
file(GLOB_RECURSE systems_src "${CMAKE_SOURCE_DIR}/systems/global/*.cpp" "${CMAKE_SOURCE_DIR}/systems/client/*.cpp")
add_executable(r-type ${src} ${systems_src})

target_link_libraries(r-type PRIVATE flux SDL3::SDL3 SDL3_ttf::SDL3_ttf SDL3_image::SDL3_image)
target_include_directories(r-type PRIVATE ${CMAKE_SOURCE_DIR}/include/)

set_target_properties(flux
        PROPERTIES PREFIX "${CMAKE_SOURCE_DIR}/"
)

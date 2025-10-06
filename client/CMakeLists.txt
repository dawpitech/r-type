cmake_minimum_required(VERSION 3.14)

file(GLOB_RECURSE src "src/*.cpp")
file(GLOB_RECURSE common_src "${CMAKE_SOURCE_DIR}/common/src/*.cpp")
add_executable(r-type ${src} ${common_src})

target_link_libraries(r-type PRIVATE flux SDL3::SDL3 SDL3_ttf::SDL3_ttf SDL3_image::SDL3_image)

target_include_directories(r-type PRIVATE
        ${CMAKE_SOURCE_DIR}/common/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_definitions(r-type PRIVATE IS_CLIENT)

set_target_properties(flux
        PROPERTIES PREFIX "${CMAKE_SOURCE_DIR}/"
)

cmake_minimum_required(VERSION 3.10)

file(GLOB_RECURSE src "src/*.cpp")
file(GLOB_RECURSE systems_src "${CMAKE_SOURCE_DIR}/systems/global/*.cpp")
add_executable(r-type ${src} ${systems_src})

target_link_libraries(r-type PRIVATE flux)
target_include_directories(r-type PRIVATE ${CMAKE_SOURCE_DIR}/include/)
target_include_directories(r-type PRIVATE ${CMAKE_SOURCE_DIR}/lib/utils/)

set_target_properties(flux
        PROPERTIES PREFIX "${CMAKE_SOURCE_DIR}/"
)

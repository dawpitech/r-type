cmake_minimum_required(VERSION 3.10)

file(GLOB_RECURSE src "src/*.cpp")
add_executable(r-server ${src})

target_link_libraries(r-server PRIVATE flux)
target_include_directories(r-server PRIVATE ${CMAKE_SOURCE_DIR}/include/global)
target_include_directories(r-server PRIVATE ${CMAKE_SOURCE_DIR}/include/server)

set_target_properties(flux
        PROPERTIES PREFIX "${CMAKE_SOURCE_DIR}/"
)

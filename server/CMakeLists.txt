cmake_minimum_required(VERSION 3.10)

file(GLOB_RECURSE src "src/*.cpp")
add_executable(r-type_server ${src})

find_package(Boost REQUIRED COMPONENTS program_options)

target_link_libraries(r-type_server PRIVATE flux Boost::program_options)
target_include_directories(r-type_server PRIVATE ${CMAKE_SOURCE_DIR}/include/global)
target_include_directories(r-type_server PRIVATE ${CMAKE_SOURCE_DIR}/include/server)
target_include_directories(r-type_server PRIVATE ${CMAKE_SOURCE_DIR}/lib/utils/)

set_target_properties(flux
        PROPERTIES PREFIX "${CMAKE_SOURCE_DIR}/"
)

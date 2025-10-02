cmake_minimum_required(VERSION 3.14)

file(GLOB_RECURSE src "src/*.cpp")
add_executable(r-type_server ${src})

target_link_libraries(r-type_server PRIVATE flux Boost::program_options Boost::asio Boost::uuid)

target_include_directories(r-type_server PRIVATE
    ${CMAKE_SOURCE_DIR}/include/global
    ${CMAKE_CURRENT_SOURCE_DIR}/src/
)

set_target_properties(flux
    PROPERTIES PREFIX "${CMAKE_SOURCE_DIR}/"
)

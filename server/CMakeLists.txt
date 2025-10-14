cmake_minimum_required(VERSION 3.14)

file(GLOB_RECURSE src "src/*.cpp")
file(GLOB_RECURSE common_src "${CMAKE_SOURCE_DIR}/common/src/*.cpp")
add_executable(r-type_server ${src} ${common_src})

target_link_libraries(r-type_server PRIVATE flux Boost::program_options Boost::asio Boost::uuid raylib raylib_cpp)

target_include_directories(r-type_server PRIVATE
    ${CMAKE_SOURCE_DIR}/common/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src/
)

set_target_properties(flux
    PROPERTIES PREFIX "${CMAKE_SOURCE_DIR}/"
)

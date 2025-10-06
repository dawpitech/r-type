cmake_minimum_required(VERSION 3.14)

file(GLOB_RECURSE src "src/*.cpp")
file(GLOB_RECURSE systems_src "${CMAKE_SOURCE_DIR}/systems/global/*.cpp" "${CMAKE_SOURCE_DIR}/systems/client/*.cpp")
add_executable(r-type ${src} ${systems_src})

target_link_libraries(r-type PRIVATE flux Boost::program_options Boost::asio Boost::uuid SDL3::SDL3 SDL3_ttf::SDL3_ttf SDL3_image::SDL3_image)

target_include_directories(r-type PRIVATE
        ${CMAKE_SOURCE_DIR}/include/global
        ${CMAKE_CURRENT_SOURCE_DIR}/src/
        ${CMAKE_SOURCE_DIR}/include/
)

set_target_properties(flux
        PROPERTIES PREFIX "${CMAKE_SOURCE_DIR}/"
)

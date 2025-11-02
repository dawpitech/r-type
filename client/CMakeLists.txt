cmake_minimum_required(VERSION 3.14)

file(GLOB_RECURSE src "src/*.cpp")
file(GLOB_RECURSE common_src "${CMAKE_SOURCE_DIR}/common/src/*.cpp")
add_executable(r-type ${src} ${common_src})

target_link_libraries(r-type PRIVATE flux Boost::program_options Boost::asio Boost::uuid raylib raylib_cpp zlib sol2 lua::lua LDtkLoader::LDtkLoader)

target_include_directories(r-type PRIVATE
        ${CMAKE_SOURCE_DIR}/common/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_definitions(r-type PRIVATE IS_CLIENT)

set_target_properties(flux
        PROPERTIES PREFIX "${CMAKE_SOURCE_DIR}/"
)

if(WIN32)
    target_compile_definitions(r-type PRIVATE NOGDI NOUSER WIN32_LEAN_AND_MEAN)

    set(PROJECT_DIR ${CMAKE_SOURCE_DIR})
    set(BUILD_DIR ${CMAKE_BINARY_DIR})
    set(CLIENT_DIR "${BUILD_DIR}/client/${CMAKE_BUILD_TYPE}")

    set(TARGET_DIR "${PROJECT_DIR}")

    add_custom_command(TARGET r-type POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CLIENT_DIR}/r-type.exe
        ${TARGET_DIR}/r-type.exe
    )
endif()

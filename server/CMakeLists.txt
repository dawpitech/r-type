cmake_minimum_required(VERSION 3.14)

file(GLOB_RECURSE src "src/*.cpp")
file(GLOB_RECURSE common_src "${CMAKE_SOURCE_DIR}/common/src/*.cpp")
add_executable(r-type_server ${src} ${common_src})

target_link_libraries(r-type_server PRIVATE flux Boost::program_options Boost::asio Boost::uuid raylib raylib_cpp zlib LDtkLoader::LDtkLoader sqlite3)

target_include_directories(r-type_server PRIVATE
    ${CMAKE_SOURCE_DIR}/common/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src/
)

set_target_properties(flux
    PROPERTIES PREFIX "${CMAKE_SOURCE_DIR}/"
)

if(WIN32)
    target_compile_definitions(r-type_server PRIVATE NOGDI NOUSER WIN32_LEAN_AND_MEAN)

    set(PROJECT_DIR ${CMAKE_SOURCE_DIR})
    set(BUILD_DIR ${CMAKE_BINARY_DIR})
    set(ZLIB_DLL_DIR "${BUILD_DIR}/_deps/zlib-build/${CMAKE_BUILD_TYPE}")
    set(SERVER_DIR "${BUILD_DIR}/server/${CMAKE_BUILD_TYPE}")

    set(TARGET_DIR "${PROJECT_DIR}")

    add_custom_command(TARGET r-type_server POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${ZLIB_DLL_DIR}/zlib.dll
        ${TARGET_DIR}/zlib.dll
    )

    add_custom_command(TARGET r-type_server POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${SERVER_DIR}/r-type_server.exe
        ${TARGET_DIR}/r-type_server.exe
    )
endif()

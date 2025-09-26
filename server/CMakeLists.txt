cmake_minimum_required(VERSION 3.14)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

FetchContent_Declare(
  Boost
  URL https://github.com/boostorg/boost/releases/download/boost-1.83.0/boost-1.83.0.tar.gz
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

FetchContent_GetProperties(Boost)
if(NOT Boost_POPULATED)
    FetchContent_Populate(Boost)

    set(Boost_SOURCE_DIR ${boost_SOURCE_DIR})
    set(Boost_BINARY_DIR ${boost_BINARY_DIR})

    if(NOT EXISTS ${Boost_SOURCE_DIR}/b2)
        execute_process(
        COMMAND ./bootstrap.sh
        WORKING_DIRECTORY ${Boost_SOURCE_DIR}
    )
    endif()

    execute_process(
        COMMAND ./b2 --with-program_options stage
        WORKING_DIRECTORY ${Boost_SOURCE_DIR}
    )
endif()

include_directories(${Boost_SOURCE_DIR})
link_directories(${Boost_SOURCE_DIR}/stage/lib)

file(GLOB_RECURSE src "src/*.cpp")
add_executable(r-type_server ${src})

target_link_libraries(r-type_server PRIVATE flux boost_program_options)

target_include_directories(r-type_server PRIVATE
    ${CMAKE_SOURCE_DIR}/include/global
    ${CMAKE_SOURCE_DIR}/lib/utils/
    ${CMAKE_CURRENT_SOURCE_DIR}/src/
)

set_target_properties(flux
    PROPERTIES PREFIX "${CMAKE_SOURCE_DIR}/"
)


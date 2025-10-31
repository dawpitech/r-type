cmake_minimum_required(VERSION 3.10)

project(R-Type
        VERSION 1.1.0
        DESCRIPTION "Touhou but with aliens"
        LANGUAGES CXX
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
#set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
set(CMAKE_CXX_STANDARD 20 CACHE STRING "C++ standard")
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CPM_USE_LOCAL_PACKAGES ON)

#set(BOOST_INCLUDE_LIBRARIES program_options)

include(cmake/CPM.cmake)
CPMUsePackageLock(package-lock.cmake)
CPMGetPackage(lua)
CPMGetPackage(sol2)
CPMGetPackage(boost)
CPMGetPackage(raylib)
CPMGetPackage(raylib-cpp)
CPMGetPackage(zlib)
CPMGetPackage(ldtk_loader)
CPMGetPackage(sqlite3)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    if (MSVC)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zi /Od")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /DEBUG")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g3 -O0")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -g3 -O0")
    endif()
endif()

add_subdirectory(lib/flux)

add_subdirectory(client)
add_subdirectory(server)

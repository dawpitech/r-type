# lua
CPMDeclarePackage(lua
        GIT_TAG 5.4.8
        GITHUB_REPOSITORY marovira/lua
        SYSTEM YES
        EXCLUDE_FROM_ALL YES
)
# sol2
CPMDeclarePackage(sol2
        VERSION 3.3.0
        GITHUB_REPOSITORY ThePhD/sol2
        SYSTEM YES
        EXCLUDE_FROM_ALL YES
)
# boost
CPMDeclarePackage(boost
        URL https://github.com/boostorg/boost/releases/download/boost-1.89.0/boost-1.89.0-cmake.tar.gz
        SYSTEM YES
        EXCLUDE_FROM_ALL YES
)
# raylib
CPMDeclarePackage(raylib
        GIT_TAG 5.5
        GITHUB_REPOSITORY raysan5/raylib
        SYSTEM YES
        EXCLUDE_FROM_ALL YES
)
# raylib-cpp
CPMDeclarePackage(raylib-cpp
        VERSION 5.5.0
        GITHUB_REPOSITORY RobLoach/raylib-cpp
        SYSTEM YES
        EXCLUDE_FROM_ALL YES
)

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
# sdl3
CPMDeclarePackage(sdl3
        GIT_TAG release-3.2.22
        GITHUB_REPOSITORY libsdl-org/SDL
        SYSTEM YES
        EXCLUDE_FROM_ALL YES
)
# sdl3_ttf
CPMDeclarePackage(sdl3_ttf
        GIT_TAG release-3.2.0
        GITHUB_REPOSITORY libsdl-org/SDL_ttf
        SYSTEM YES
        EXCLUDE_FROM_ALL YES
)

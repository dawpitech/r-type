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
        URL https://github.com/libsdl-org/SDL/releases/download/release-3.2.20/SDL3-3.2.20.tar.gz
        SYSTEM YES
        EXCLUDE_FROM_ALL YES
)
# sdl3_ttf
CPMDeclarePackage(sdl3_ttf
        URL https://github.com/libsdl-org/SDL_ttf/releases/download/release-3.2.2/SDL3_ttf-3.2.2.tar.gz
        SYSTEM YES
        EXCLUDE_FROM_ALL YES
)
# sdl3_image
CPMDeclarePackage(sdl3_image
        URL https://github.com/libsdl-org/SDL_image/releases/download/release-3.2.4/SDL3_image-3.2.4.tar.gz
        SYSTEM YES
        EXCLUDE_FROM_ALL YES
)

set export

OUTDIR := env("BUILD_OUT", "build")
DEBUG_DIR := env("DEBUG_DIR", "cmake-build-cache")
CPM_SOURCE_CACHE := env("CPM_SOURCE_CACHE", ".cpm_cache")
ASAN_FLAGS := '-DCMAKE_CXX_FLAGS="-fsanitize=address -g" -DCMAKE_EXE_LINKER_FLAGS="-fsanitize=address"'

release: (_build "Release")

debug: (_build "Debug")

asan: (_build "Debug" ASAN_FLAGS)

_build type extra='':
    cmake -S . -B {{ OUTDIR }} -DCMAKE_BUILD_TYPE={{ type }} {{extra}}
    cmake --build {{ OUTDIR }} -j {{ num_cpus() }}
    cp {{ OUTDIR }}/client/r-type .
    cp {{ OUTDIR }}/server/r-type_server .

clean:
    rm -rf {{ OUTDIR }}
    rm -rf {{ DEBUG_DIR }}
    rm -f r-type
    rm -f r-type_server

purge: clean
    rm -rf .cpm_cache

re: clean release

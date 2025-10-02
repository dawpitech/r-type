set export

OUTDIR := env("BUILD_OUT", "build")
CPM_SOURCE_CACHE := env("CPM_SOURCE_CACHE", "${PWD}/.cpm_cache")

release: (_build "Release")

debug: (_build "Debug")

_build type:
    cmake -S . -B {{ OUTDIR }} -DCMAKE_BUILD_TYPE={{ type }}
    cmake --build {{ OUTDIR }} -j {{ num_cpus() }}

clean:
    rm -rf {{ OUTDIR }}
    rm -f r-type
    rm -rf .cpm_cache

re: clean release

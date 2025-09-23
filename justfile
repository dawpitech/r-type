outdir := env("BUILD_OUT", "build")

release: (_build "Release")

debug: (_build "Debug")

_build type:
    cmake -S . -B {{ outdir }} -DCMAKE_BUILD_TYPE={{ type }}
    cmake --build {{ outdir }}  -j {{ num_cpus() }}

clean:
    rm -rf {{ outdir }}
    rm -f r-type

re: clean release

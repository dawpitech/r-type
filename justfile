outdir := env("BUILD_OUT", "build")

release: (_build "Release" "NONE")

_build type sanitizer:
    cmake -S . -B {{ outdir }} -DCMAKE_BUILD_TYPE={{ type }} -DSANITIZER={{ sanitizer }}
    cmake --build {{ outdir }}  -j {{ num_cpus() }}

clean:
    rm -rf {{ outdir }}
    rm -f r-type

re: clean release


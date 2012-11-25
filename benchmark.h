#ifndef BENCHMARK_H_INCLUDED
#define BENCHMARK_H_INCLUDED

//========================================================================
// MemoryPool - dependancies
//========================================================================

#include <sys/time.h>

//========================================================================
// MemoryPool - definitions
//========================================================================

    template <typename Func>
    double benchmark(Func f, size_t iterations);

//========================================================================
// MemoryPool - implementations
//========================================================================

template <typename Func>
double benchmark(Func f, size_t iterations, char* path) {
//    f( path );

    timeval a, b;
    gettimeofday(&a, 0);

    for (; iterations --> 0;) {
        f( path );
    }

    gettimeofday(&b, 0);
    return ((double)( (b.tv_sec * (unsigned int)1e6 + b.tv_usec) -
        (a.tv_sec * (unsigned int)1e6 + a.tv_usec) ) ) ;
}

#endif // BENCHMARK_H_INCLUDED

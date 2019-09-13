#ifndef HASHMAP_TEST_COMMON_H
#define HASHMAP_TEST_COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

#include "tests/test.h"

#include <string.h>
#include <sys/types.h>

typedef void (*testfunc)(void);

/**
 * compares two string arrays that can be out of order
 */
#define ASSERT_STR_ARR_EQ(A1, A2, LEN)                                                            \
    {                                                                                             \
        for (i = 0; i < LEN; i++)                                                                 \
        {                                                                                         \
            for (k = 0; k < LEN; k++)                                                             \
            {                                                                                     \
                if (strcmp(A1[i], A2[k]) == 0)                                                    \
                {                                                                                 \
                    goto Found;                                                                   \
                }                                                                                 \
            }                                                                                     \
            assert(0);                                                                            \
        Found:;                                                                                   \
        }                                                                                         \
    }

extern char* randstring(size_t);
extern char** rand_keys(size_t n);
extern void free_string_arr(char** arr, int len);
extern void Benchmark(const char*, bench_func);
extern void AverageBenchmark(const char*, bench_func, size_t);
extern void Benchmarkf(const char*, bench_func, const char*);
extern void nBenchmark(const char*, bench_func, size_t);
extern int Run(testfunc*, int);

#ifdef __cplusplus
}
#endif

#endif /* HASHMAP_TEST_COMMON_H */
#ifndef HASHMAP_TEST_COMMON_H
#define HASHMAP_TEST_COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

#include <assert.h>
#include <string.h>
#include <sys/types.h>

typedef void (*testfunc)(void);

#ifdef assert
#undef assert
#endif

// because i like pretty colors :)
/*
#define assert(exp) \
    ((exp) ? (void)(0) \
           : __assert_fail("\033[0;31m" #exp "\033[0m", "\033[0;31m" __FILE__ "\033[0m", \
                           __LINE__, __ASSERT_FUNCTION))
*/
int assertion_failure(const char* fmt, ...);

#define assert(exp)                                                                               \
    ((exp) ? (void)(0) : assertion_failure("%s:%d \"%s\"\n", __FILE__, __LINE__, #exp));

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

struct testable
{
    char* name;
    int (*test_fn)(void);
};

#define assert_eq(C1, C2)                                                                         \
    if ((C1) != (C2))                                                                             \
    {                                                                                             \
        printf("\n\033[0;31mAssertion failed: %s:%d\033[0m \"%s and %s\"\n", __FILE__, __LINE__,  \
               #C1, #C2);                                                                         \
        result = 1;                                                                               \
        goto EndOfTest;                                                                           \
    }

// clang-format off
#define ADD_TEST(NAME) { #NAME, test_##NAME }

#define TEST(NAME, CLOSURE)                                                                       \
    int test_##NAME(void)                                                                         \
    {                                                                                             \
        int result = 0;                                                                           \
        CLOSURE;                                                                                  \
    EndOfTest:                                                                                    \
        return result;                                                                            \
    }

#define TEST_SUITE(...) static struct testable _main_tests[] = { __VA_ARGS__, { NULL, NULL } };

#define RUN_TEST_SUITE(...)                                                                       \
    TEST_SUITE(__VA_ARGS__);                                                                      \
    int main() { return RunAllTests(_main_tests); }

int RunAllTests(struct testable* tests);
#define RunTests() RunAllTests(_main_tests);

extern char* randstring(size_t);
extern char** rand_keys(size_t n);
extern void free_string_arr(char** arr, int len);
extern double benchmark(testfunc);
extern double nbenchmark(testfunc, int);
extern void Benchmark(const char*, testfunc);
extern void AverageBenchmark(const char*, testfunc, size_t);
extern void Benchmarkf(const char*, testfunc, const char*);
extern void nBenchmark(const char*, testfunc, size_t);
extern int Run(testfunc*, int);

#ifdef __cplusplus
}
#endif

#endif /* HASHMAP_TEST_COMMON_H */
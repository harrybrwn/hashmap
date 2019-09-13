#ifndef _TEST_H
#define _TEST_H

#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <sys/timeb.h>

typedef void (*bench_func)(void);

struct testable
{
    char* name;
    int (*test_fn)(void);
};

struct benchmark
{
    char* name;
    bench_func bench_fn;
};

#ifdef COMPILE_TESTS_H
int assertion_failure(const char* fmt, ...)
{
    char fmtbuf[256];
    va_list args;
    snprintf(fmtbuf, sizeof(fmtbuf), "\033[0;31mAssertion Failure:\033[0m %s", fmt);

    va_start(args, fmt);
    vfprintf(stderr, fmtbuf, args);
    va_end(args);

    raise(SIGABRT);
    return -1;
}

int RunAllTests(struct testable* test_list)
{
    struct testable t;
    int i = 0, failure = 0, result;

    while ((t = test_list[i++]).name != NULL)
    {
        result = t.test_fn();
        if (result != 0)
        {
            printf("%s failed!\n", t.name);
            failure = 1;
        }
        else
        {
            printf(".");
        }
    }

    if (failure)
        printf("\n\033[0;31mFAILURE\033[0m\n");
    else
        printf("\n\033[0;32mPASS\033[0m\n");
    return failure;
}

int RunAllBenchmarks(struct benchmark* bench_list)
{
    struct benchmark t;
    int i = 0;
    // double result;

    t = bench_list[i++];
    while (t.name != NULL && t.bench_fn != NULL)
    {
        t.bench_fn();
        // result = nbenchmark(t.bench_fn, n);
        // printf("  %s: %.3f sec\n", t.name, result);
        t = bench_list[i++];
    }
    return 0;
}
#else
int assertion_failure(const char* fmt, ...);
int RunAllTests(struct testable* test_list);
int RunAllBenchmarks(struct benchmark* bench_list);
#endif

#ifdef assert
#undef assert
#endif

#define assert(exp)                                                                               \
    ((exp) ? (void)(0) : assertion_failure("%s:%d \"%s\"\n", __FILE__, __LINE__, #exp));

#define assert_eq(C1, C2)                                                                         \
    if ((C1) != (C2))                                                                             \
    {                                                                                             \
        printf("\n\033[0;31mAssertion failed: %s:%d\033[0m \"%s and %s\"\n", __FILE__, __LINE__,  \
               #C1, #C2);                                                                         \
        result = 1;                                                                               \
        goto EndOfTest;                                                                           \
    }

#define TEST(NAME, CLOSURE)                                                                       \
    static int test_##NAME(void)                                                                  \
    {                                                                                             \
        int result = 0;                                                                           \
        CLOSURE;                                                                                  \
    EndOfTest:                                                                                    \
        return result;                                                                            \
    }

#define BENCH(NAME, CLOSURE)                                                                      \
    static void benchmark_##NAME(void)                                                            \
    {                                                                                             \
        struct timeb start, end;                                                                  \
        ftime(&start);                                                                            \
        (CLOSURE);                                                                                \
        ftime(&end);                                                                              \
        double diff =                                                                             \
          (double)(1000.0 * (end.time - start.time) + (end.millitm - start.millitm)) / 1000;      \
        printf("  %s: %.3f sec\n", #NAME, diff);                                                  \
    }

// clang-format off
#define ADD_TEST(NAME) { #NAME, test_##NAME }
#define ADD_BENCH(NAME) { #NAME, benchmark_##NAME }

#define TEST_SUITE(...) static struct testable _main_tests[] = { __VA_ARGS__, { NULL, NULL } };
#define BENCHMARK_SUITE(...) static struct benchmark _main_benchmarks[] = { __VA_ARGS__, { NULL, NULL} };

#define RunTests() RunAllTests(_main_tests)
#define RunBenchmarks() RunAllBenchmarks(_main_benchmarks)

#define RUN_TEST_SUITE(...)                                                                       \
    TEST_SUITE(__VA_ARGS__);                                                                      \
    int main() { return RunAllTests(_main_tests); }

#define RUN_BENCH_SUITE(...)                \
    BENCHMARK_SUITE(__VA_ARGS__);           \
    int main() { return RunBenchmarks(); }

#endif /* _TEST_H */
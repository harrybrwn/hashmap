#ifndef _TEST_H
#define _TEST_H

#if !defined(_UTEST_H)

#ifdef __cplusplus
extern "C" {
#endif

#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <sys/timeb.h>

typedef int (*test_func)(void);
typedef void (*bench_func)(void);

struct testable
{
    char* name;
    test_func test_fn;
};

struct benchmark
{
    char* name;
    bench_func bench_fn;
};

int assertion_failure_nodump(const char* fmt, ...)
{
    char fmtbuf[256];
    va_list args;
    snprintf(fmtbuf, sizeof(fmtbuf), "\033[0;31mAssertion Failure:\033[0m %s", fmt);

    va_start(args, fmt);
    vfprintf(stderr, fmtbuf, args);
    va_end(args);
    return -1;
}

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
        printf("\n\033[0;31mFAILURE\033[0m\n\n");
    else
        printf("\n\033[0;32mPASS\033[0m\n\n");
    return failure;
}

int RunAllBenchmarks(struct benchmark* bench_list)
{
    struct benchmark t;
    int i = 0;

    t = bench_list[i++];
    while (t.name != NULL && t.bench_fn != NULL)
    {
        t.bench_fn();
        t = bench_list[i++];
    }
    return 0;
}

#ifndef assert
#undef assert
#define assert(exp)                                                                               \
    if (!(exp))                                                                                   \
    {                                                                                             \
        assertion_failure_nodump("%s:%d \"%s\"\n", __FILE__, __LINE__, #exp);                     \
        result = 1;                                                                               \
        goto EndOfTest;                                                                           \
    }
#endif

#define assert_eq(C1, C2)                                                                         \
    if ((C1) != (C2))                                                                             \
    {                                                                                             \
        printf("\n\033[0;31mAssertion failed: %s:%d\033[0m \"%s and %s\"\n", __FILE__, __LINE__,  \
               #C1, #C2);                                                                         \
        result = 1;                                                                               \
        goto EndOfTest;                                                                           \
    }

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
            assertion_failure("ASSERT_STR_ARR_EQ failed: (%s:%d)\n", __FILE__, __LINE__);         \
        Found:;                                                                                   \
        }                                                                                         \
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
#define ADD_TEST(NAME) { (char*) #NAME, test_##NAME }
#define ADD_BENCH(NAME) { (char*) #NAME, benchmark_##NAME }

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

/************************************************************************
 Everything below is just my personal utilities.

 above if my testing framework.
*************************************************************************/
#endif /* !defined(_UTEST_H) */

#include <stdlib.h>

char* randstring(size_t length)
{
    static char charset[] =
      "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789,.-#'?!";
    char* str = NULL;
    if (length)
    {
        str = (char*)malloc(sizeof(char) * (length + 1));
        if (str)
        {
            size_t n;
            int key;
            for (n = 0; n < length; n++)
            {
                key = rand() % (int)(sizeof(charset) - 1);
                str[n] = charset[key];
            }
            str[length] = '\0';
        }
    }
    return str;
}

char** rand_keys(size_t n)
{
    char** keys = (char**)malloc(sizeof(char*) * n);
    size_t i;
    for (i = 0; i < n; i++)
    {
        keys[i] = randstring(10);
    }
    return keys;
}

void free_string_arr(char** arr, int len)
{
    int i;
    for (i = 0; i < len; i++)
        free(arr[i]);
    free(arr);
}

#ifdef __cplusplus
}
#endif

#endif /* _TEST_H */
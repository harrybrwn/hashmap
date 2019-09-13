#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/timeb.h>
#include <time.h>

#ifndef _WIN32
#include <sys/resource.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#include "tests/test_common.h"

char* randstring(size_t length)
{
    static char charset[] =
      "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789,.-#'?!";
    char* str = NULL;
    if (length)
    {
        str = malloc(sizeof(char) * (length + 1));
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
    char** keys = malloc(sizeof(char*) * n);
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

#if !defined(_WIN32) && !defined(__GNUC__)
/* this struct is nowhere to be found in the standard library */
struct timezone
{
    int tz_minuteswest;
    int tz_dsttime;
};
#endif

double get_time()
{
    struct timeval t;
    struct timezone tzp;
    gettimeofday(&t, &tzp);
    return t.tv_sec + t.tv_usec * 1e-6;
}

double benchmark(testfunc testfn)
{
    struct timeb start, end;
    ftime(&start);

    testfn();

    ftime(&end);

    double diff =
      (double)(1000.0 * (end.time - start.time) + (end.millitm - start.millitm)) / 1000;
    return diff;
}

double nbenchmark(testfunc testfn, int n)
{
    struct timeb start, end;
    ftime(&start);
    int i;
    for (i = 0; i < n; i++)
        testfn();

    ftime(&end);

    double diff =
      (double)(1000.0 * (end.time - start.time) + (end.millitm - start.millitm)) / 1000;
    return diff;
}

void Benchmark(const char* name, testfunc testfn)
{
    double result = benchmark(testfn);
    printf("  %s: %.3f sec\n", name, result);
}

void Benchmarkf(const char* name, testfunc testfn, const char* msg)
{
    double result = benchmark(testfn);
    printf("  %s: %s %.3f\n", name, msg, result);
}

void AverageBenchmark(const char* name, testfunc testfn, size_t n)
{
    double result = nbenchmark(testfn, n);
    printf("  %s:\n    total: %.3f\n    average: %.3f\n", name, result, result / (double)n);
}

void nBenchmark(const char* name, testfunc testfn, size_t n)
{
    float result = nbenchmark(testfn, n);
    printf("  %s x %lu: %.3f\n", name, n, result);
}

int Run(testfunc* tests, int n)
{
    int i;
    for (i = 0; i < n; i++)
    {
        tests[i]();
        printf(".");
    }
    printf("\n----------------------------------------------------------------------"
           "\n");
    printf("Ran %d tests\n", n);
    printf("\nOK\n\n");
    return 0;
}

int RunAllTests(struct testable* tests)
{
    struct testable t;
    int i = 0, failure = 0, result;

    while ((t = tests[i++]).name != NULL)
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

#define ERR_FMT "\033[0;31m%s\033[0m"

static void verrorf(const char* outer_fmt, const char* fmt, va_list args)
{
    char fmtbuf[256];
    snprintf(fmtbuf, sizeof(fmtbuf), outer_fmt, fmt);
    vfprintf(stderr, fmtbuf, args);
}

// errorf will print out an error and exit the program with the given
// status code. This function will not exit.
void errorf(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    verrorf(ERR_FMT, fmt, args);
    va_end(args);
}

int assertion_failure(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    verrorf("\033[0;31mAssertion Failure:\033[0m %s", fmt, args);
    va_end(args);
    raise(SIGABRT);
    return -1;
}

#ifdef __cplusplus
}
#endif
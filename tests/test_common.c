#include <stdlib.h>
#include <stdio.h>

#include <sys/timeb.h>
#include <sys/time.h>

#ifndef _WIN32
#include <sys/resource.h>
#endif

#include "tests/test_common.h"

char* randstring(size_t length)
{
    static char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789,.-#'?!";
    char *str = NULL;
	int key;
    if (length) {
        str = malloc(sizeof(char) * (length + 1));
        if (str) {
            for (int n = 0;n < length;n++) {
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
	char** keys = malloc(sizeof(char*)*n);
	size_t i;
    for (i = 0; i < n; i++) {
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

#ifndef _WIN32
/* this struct is nowhere to be found in the standard library */
struct timezone {
	int tz_minuteswest;
	int tz_dsttime;
};
#endif

double get_time()
{
	struct timeval t;
    struct timezone tzp;
    gettimeofday(&t, &tzp);
    return t.tv_sec + t.tv_usec*1e-6;
}

double benchmark(testfunc testfn)
{
	struct timeb start, end;
	ftime(&start);

	testfn();

	ftime(&end);

	double diff = (double)(1000.0 * (end.time - start.time)
		+ (end.millitm - start.millitm))/1000;
	return diff;
}

double nbenchmark(testfunc testfn, int n)
{
	struct timeb start, end;
	ftime(&start);

	for (int i = 0; i < n; i++)
		testfn();

	ftime(&end);

	double diff = (double)(1000.0 * (end.time - start.time)
		+ (end.millitm - start.millitm))/1000;
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
	printf("  %s:\n    total: %.3f\n    average: %.3f\n", name, result, result/(double)n);
}

void nBenchmark(const char* name, testfunc testfn, size_t n)
{
	float result = nbenchmark(testfn, n);
	printf("  %s x %lu: %.3f\n", name, n, result);
}
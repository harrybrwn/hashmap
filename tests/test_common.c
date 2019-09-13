#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/timeb.h>
#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

// #include "tests/test_common.h"

// char* randstring(size_t length)
// {
//     static char charset[] =
//       "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789,.-#'?!";
//     char* str = NULL;
//     if (length)
//     {
//         str = malloc(sizeof(char) * (length + 1));
//         if (str)
//         {
//             size_t n;
//             int key;
//             for (n = 0; n < length; n++)
//             {
//                 key = rand() % (int)(sizeof(charset) - 1);
//                 str[n] = charset[key];
//             }
//             str[length] = '\0';
//         }
//     }
//     return str;
// }

// char** rand_keys(size_t n)
// {
//     char** keys = malloc(sizeof(char*) * n);
//     size_t i;
//     for (i = 0; i < n; i++)
//     {
//         keys[i] = randstring(10);
//     }
//     return keys;
// }

// void free_string_arr(char** arr, int len)
// {
//     int i;
//     for (i = 0; i < len; i++)
//         free(arr[i]);
//     free(arr);
// }

// int Run(testfunc* tests, int n)
// {
//     int i;
//     for (i = 0; i < n; i++)
//     {
//         tests[i]();
//         printf(".");
//     }
//     printf("\n----------------------------------------------------------------------"
//            "\n");
//     printf("Ran %d tests\n", n);
//     printf("\nOK\n\n");
//     return 0;
// }

#ifdef __cplusplus
}
#endif
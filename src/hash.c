#include "hashmap.h"

hash_t djb2(char* str)
{
    hash_t hash = 5381;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;

    return hash;
}

hash_t sdbm(char* str)
{
    hash_t hash = 0;
    int c;

    while ((c = *str++))
        hash = c + (hash << 6) + (hash << 16) - hash;

    return hash;
}

hash_t rshash(char* str)
{
    hash_t a = 63689, b = 378551, hash = 0;
    int c;

    while ((c = *str++))
    {
        hash = hash * a + c;
        a = a * b;
    }
    return (hash & 0x7FFFFFFF);
}

/**
 * fnv hashing specification found at http://isthe.com/chongo/tech/comp/fnv/
 */
#if __x86_64__ || __ppc64__ // if compiled for 64 bit os
#define FNV_PRIME 0x100000001b3UL
#define FNV_PRIME_MUL(X) (X << 1) + (X << 4) + (X << 5) + (X << 7) + (X << 8) + (X << 40)
#define FNV_OFFSET 0xcbf29ce484222325UL
#else // for 32 bit machines
#define FNV_PRIME 16777619UL
#define FNV_PRIME_MUL(X) (X << 1) + (X << 4) + (X << 7) + (X << 8) + (X << 24)
#define FNV_OFFSET 2166136261UL
#endif

hash_t fnv_1(char* str)
{
    unsigned char* s = (unsigned char*)str;
    hash_t hval = FNV_OFFSET;

    while (*s)
    {
        hval += FNV_PRIME_MUL(hval);
        hval ^= (hash_t)*s++;
    }
    return hval;
}

hash_t fnv_1a(char* str)
{
    unsigned char* s = (unsigned char*)str;
    hash_t hval = FNV_OFFSET;

    while (*s)
    {
        hval ^= (hash_t)*s++;
        hval += FNV_PRIME_MUL(hval);
    }
    return hval;
}
#include <stdio.h>
#include <string.h>
#include <assert.h>

#ifdef _WIN32
#pragma warning(disable : 4996)	// deprecated CRT functions (_CRT_SECURE_NO_WARNINGS).
#endif

// These two functions courtesy Knightmare.

// Safely copies source string into destination string.
// Returns the number of characters copied into the destination
// if successful or returns the number of characters that would have
// been copied if there was room in the destination. Returns 0 on bad input.
// Always nul-terminates the destination.
size_t Q_strncpyz(char* dst, size_t dstSize, const char* src)
{
    char* d = dst;
    const char* s = src;
    size_t        decSize = dstSize;

    if (!dst || !src || dstSize < 1) {
        return 0;
    }

    while (--decSize && *s)
        *d++ = *s++;
    *d = 0;

    if (decSize == 0)    // Insufficent room in dst, return count + length of remaining src
        return (s - src + strlen(s));
    else
        return (s - src);    // returned count excludes NUL terminator
}

// Safely concatenates C strings.
// Returns the number of characters in the destination if successful. 
// Returns the number of characters that would have been in the destination
// if there was room in the destination. Returns 0 on bad input.
// Always nul-terminates the destination.
size_t Q_strncatz(char* dst, size_t dstSize, const char* src)
{
    char* d = dst;
    const char* s = src;
    size_t        decSize = dstSize;
    size_t        dLen;

    if (!dst || !src || dstSize < 1) {
        return 0;
    }

    while (--decSize && *d)
        d++;
    dLen = d - dst;

    if (decSize == 0)
        return (dLen + strlen(s));

    if (decSize > 0) {
        while (--decSize && *s)
            *d++ = *s++;

        *d = 0;
    }

    return (dLen + (s - src));    // returned count excludes NULL terminator
}

/*
 * Copy src to string dst of size siz.  At most siz-1 characters
 * will be copied.  Always NUL terminates (unless siz == 0).
 * Returns strlen(src); if retval >= siz, truncation occurred.
 */
size_t Q_strlcpy(char* dst, const char* src, size_t siz)
{
    char* d = dst;
    const char* s = src;
    size_t n = siz;

    /* Copy as many bytes as will fit */
    if (n != 0 && --n != 0)
    {
        do
        {
            if ((*d++ = *s++) == 0)
                break;
        } while (--n != 0);
    }

    /* Not enough room in dst, add NUL and traverse rest of src */
    if (n == 0)
    {
        if (siz != 0)
            *d = '\0'; /* NUL-terminate dst */
        while (*s++)
            ; // counter loop
    }

    return (s - src - 1); /* count does not include NUL */
}

int main(int argc, char* argv[])
{
    size_t count, ocount;
    char dest[50];
    const char* src0 = "This proposed string is way too long for the destination.";
    const char* src1 = "magic";
    const char* src2 = " eightball";
    const char* src3 = " says \"You may rely on it.\"";

    printf("Begin testing Q_strncpyz and Q_strncatz.\n\n");
    count = Q_strncpyz(NULL, sizeof dest, src0); // Bad first arg
    assert(count == 0);
    count = Q_strncpyz(dest, 0, src0); // Bad second arg
    assert(count == 0);
    count = Q_strncpyz(dest, sizeof dest, NULL); // Bad third arg
    assert(count == 0);

    count = Q_strncatz(NULL, sizeof dest, src0); // Bad first arg
    assert(count == 0);
    count = Q_strncatz(dest, 0, src0); // Bad second arg
    assert(count == 0);
    count = Q_strncatz(dest, sizeof dest, NULL); // Bad third arg
    assert(count == 0);

    printf("Test truncation modes.\n");
    count = Q_strncpyz(dest, sizeof dest, src0);
    assert(count == strlen(src0));
    if (strlen(dest) < strlen(src0)) {
        printf("\"%s\"\n\"%s\"\n", dest, src0);
        puts("Truncation detected Q_strncpyz OK\n");
    }

    printf("Test truncation modes.\n");
    count = Q_strlcpy(dest, src0, sizeof dest);
    assert(count == strlen(src0));
    if (strlen(dest) < strlen(src0)) {
        printf("\"%s\"\n\"%s\"\n", dest, src0);
        puts("Truncation detected Q_strlcpy OK\n");
    }

    ocount = strlen(dest);
    count = Q_strncatz(dest, sizeof dest, src0);
    assert(count == strlen(dest) + strlen(src0));
    if (count < ocount + strlen(src0)) {
        printf("\"%s\"\n\"%s\"\n", dest, src0);
        puts("Truncation detected Q_strncatz OK\n");
    }

    /* Normal pathways */
    count = Q_strncpyz(dest, sizeof dest, src1);
    assert(count == strlen(src1));
    printf("%s\n", dest);
    
    count = Q_strncatz(dest, sizeof dest, src2);
    assert(count == strlen(dest));
    printf("%s\n", dest);
 
    count = Q_strncatz(dest, sizeof dest, src3);
    assert(count == strlen(dest));
    printf("%s\n\n", dest);
 
    //count = Q_strlcpy(dest, src1, sizeof dest);
    //assert(count == strlen(src1));
    //printf("%s\n", dest);

    printf("Success testing Q_strncpyz and Q_strncatz.\n");
}

/*
Begin testing Q_strncpyz and Q_strncatz.

Test truncation modes.
"This proposed string is way too long for the dest"
"This proposed string is way too long for the destination."
Q_strncpyz OK

"This proposed string is way too long for the dest"
"This proposed string is way too long for the destination."
Q_strncatz OK

magic
magic eightball
magic eightball says "You may rely on it."

Success testing Q_strncpyz and Q_strncatz.

*/
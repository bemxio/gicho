#include "stdlib.h"

int atoi(const char* nptr) {
    int n = 0;
    long m = 1;

    for (int i = strlen(nptr) - 1; i >= 0; i--) {
        n += (nptr[i] - 48) * m;
        m *= 10;
    }

    return n;
}

long atol(const char* nptr) {
    long n = 0;
    long m = 1;

    for (int i = strlen(nptr) - 1; i >= 0; i--) {
        n += (nptr[i] - 48) * m;
        m *= 10;
    }

    return n;
}

unsigned long atoul(const char* nptr) {
    unsigned long n = 0;
    long m = 1;

    for (int i = strlen(nptr) - 1; i >= 0; i--) {
        n += (nptr[i] - 48) * m;
        m *= 10;
    }

    return n;
}

char* itoa(int n, char* buf) {
    int i = 0;

    if (n == 0) {
        buf[i++] = '0';
    } else {
        for (int x = n; x > 0; x /= 10) {
            buf[i++] = x % 10 + 48;
        }
    }

    buf[i] = '\0';
    strrev(buf);

    return buf;
}
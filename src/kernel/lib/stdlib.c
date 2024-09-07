#include "stdlib.h"

unsigned long atoul(const char* nptr) {
    unsigned long n = 0;
    long m = 1;

    for (int i = strlen(nptr) - 1; i >= 0; i--) {
        if (nptr[i] < '0' || nptr[i] > '9') {
            return 0;
        }

        n += (nptr[i] - 48) * m;
        m *= 10;
    }

    return n;
}

char* ultoa(unsigned long n, char* buf) {
    int i = 0;

    if (n == 0) {
        buf[i++] = '0';
    } else {
        for (unsigned long x = n; x > 0; x /= 10) {
            buf[i++] = x % 10 + 48;
        }
    }

    buf[i] = '\0';
    strrev(buf);

    return buf;
}
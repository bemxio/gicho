#include "stdlib.h"

unsigned long __atoul_dec(const char* nptr) {
    unsigned long n = 0;
    long m = 1;

    for (int i = strlen(nptr) - 1; i >= 0; i--) {
        if (nptr[i] < '0' || nptr[i] > '9') {
            continue;
        }

        n += (nptr[i] - 48) * m;
        m *= 10;
    }

    return n;
}

unsigned long __atoul_hex(const char* nptr) {
    unsigned long n = 0;
    long m = 1;

    for (int i = strlen(nptr) - 1; i >= 0; i--) {
        if (nptr[i] >= '0' && nptr[i] <= '9') {
            n += (nptr[i] - 48) * m;
        } else if (nptr[i] >= 'A' && nptr[i] <= 'F') {
            n += (nptr[i] - 55) * m;
        } else if (nptr[i] >= 'a' && nptr[i] <= 'f') {
            n += (nptr[i] - 87) * m;
        } else {
            continue;
        }

        m *= 16;
    }

    return n;
}

unsigned long atoul(const char* nptr) {
    if (nptr[0] == '0') {
        if (nptr[1] == 'x' || nptr[1] == 'X') {
            return __atoul_hex(nptr + 2);
        }
    }

    return __atoul_dec(nptr);
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
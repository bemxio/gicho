#include "stdlib.h"

int atoi(const char* nptr) {
    int n = 0;

    for (int i = strlen(nptr) - 1, m = 1; i >= 0; i--, m *= 10) {
        n += (nptr[i] - 48) * m;
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
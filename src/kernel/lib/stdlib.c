#include "stdlib.h"

unsigned long __atoul_dec(const char* nptr) {
    unsigned long n = 0;
    long long m = 1;

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
    long long m = 1;

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

unsigned long __atoul_bin(const char* nptr) {
    unsigned long n = 0;
    int m = 1;

    for (int i = strlen(nptr) - 1; i >= 0; i--) {
        if (nptr[i] != '0' || nptr[i] != '1') {
            continue;
        }

        n += (nptr[i] - 48) * m;
        m *= 2;
    }

    return n;
}

unsigned long atoul(const char* nptr) {
    if (nptr[0] == '0') {
        if (nptr[1] == 'x' || nptr[1] == 'X') {
            return __atoul_hex(nptr + 2);
        } else if (nptr[1] == 'b' || nptr[1] == 'B') {
            return __atoul_bin(nptr + 2);
        }
    }

    return __atoul_dec(nptr);
}

char* __ultoa_dec(unsigned long n, char* buf) {
    unsigned char i = 0;

    if (n == 0) {
        buf[i++] = '0';
    } else {
        for (unsigned long x = n; x > 0; x /= 10) {
            buf[i++] = x % 10 + 48;
        }
    }

    buf[i] = '\0';

    return strrev(buf);
}

char* __ultoa_hex(unsigned long n, char* buf) {
    unsigned char i = 0;

    if (n == 0) {
        buf[i++] = '0';
    } else {
        for (unsigned long x = n; x > 0; x /= 16) {
            unsigned long r = x % 16;

            if (r < 10) {
                buf[i++] = r + 48;
            } else {
                buf[i++] = r + 55;
            }
        }
    }

    buf[i++] = 'x';
    buf[i++] = '0';
    buf[i] = '\0';

    return strrev(buf);
}

char* __ultoa_bin(unsigned long n, char* buf) {
    unsigned char i = 0;

    if (n == 0) {
        buf[i++] = '0';
    } else {
        for (unsigned long x = n; x > 0; x /= 2) {
            buf[i++] = x % 2 + 48;
        }
    }

    buf[i] = '\0';

    return strrev(buf);
}

char* ultoa(unsigned long n, char* buf, char base) {
    switch (base) {
        case 2:
            return __ultoa_bin(n, buf);
        case 10:
            return __ultoa_dec(n, buf);
        case 16:
            return __ultoa_hex(n, buf);
    }

    return NULL;
}
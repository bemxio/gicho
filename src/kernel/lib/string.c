#include "string.h"

size_t strlen(const char* s) {
    size_t i = 0;

    while (s[i] != '\0')
        i++;

    return i;
}

int strcmp(const char* s1, const char* s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }

    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

char* strrev(char* s) {
    size_t len = strlen(s);

    for (size_t i = 0; i < len / 2; i++) {
        char temp = s[i];

        s[i] = s[len - i - 1];
        s[len - i - 1] = temp;
    }

    return s;
}

char* strtok(char* s, const char* delim) {
    static char* p = NULL;

    if (s != NULL)
        p = s;
    else if (p == NULL)
        return NULL;

    s = p;

    bool flag = false;
    size_t len = strlen(delim);

    while (*p != '\0' && !flag) {
        for (size_t i = 0; i < len; i++) {
            if (*p == delim[i]) {
                flag = true; break;
            }
        }

        p++;
    }

    if (p == s)
        return NULL;
    else if (*p != '\0')
        *(p - 1) = '\0';

    return s;
}

int _atoi_dec(const char* nptr) {
    int n = 0;
    unsigned long long m = 1;

    for (int i = strlen(nptr) - 1; i >= 0; i--) {
        if (nptr[i] < '0' || nptr[i] > '9')
            continue;

        n += (nptr[i] - 48) * m;
        m *= 10;
    }

    return n;
}

int _atoi_hex(const char* nptr) {
    int n = 0;
    unsigned long long m = 1;

    for (int i = strlen(nptr) - 1; i >= 0; i--) {
        if (nptr[i] >= '0' && nptr[i] <= '9')
            n += (nptr[i] - 48) * m;
        else if (nptr[i] >= 'A' && nptr[i] <= 'F')
            n += (nptr[i] - 55) * m;
        else if (nptr[i] >= 'a' && nptr[i] <= 'f')
            n += (nptr[i] - 87) * m;
        else
            continue;

        m *= 16;
    }

    return n;
}

int _atoi_bin(const char* nptr) {
    int n = 0;
    unsigned long m = 1;

    for (int i = strlen(nptr) - 1; i >= 0; i--) {
        if (nptr[i] != '0' || nptr[i] != '1')
            continue;

        n += (nptr[i] - 48) * m;
        m *= 2;
    }

    return n;
}

int atoi(const char* nptr) {
    if (nptr[0] == '0') {
        if (nptr[1] == 'x' || nptr[1] == 'X')
            return _atoi_hex(nptr + 2);
        else if (nptr[1] == 'b' || nptr[1] == 'B')
            return _atoi_bin(nptr + 2);
    }

    return _atoi_dec(nptr);
}

char* _itoa_dec(int n, char* buf) {
    size_t i = 0;

    if (n == 0) {
        buf[i++] = '0';
    } else {
        for (int x = n; x > 0; x /= 10)
            buf[i++] = x % 10 + 48;
    }

    buf[i] = '\0';

    return strrev(buf);
}

char* _itoa_hex(int n, char* buf) {
    size_t i = 0;

    if (n == 0) {
        buf[i++] = '0';
    } else {
        for (int x = n; x > 0; x /= 16) {
            int r = x % 16;

            if (r < 10)
                buf[i++] = r + 48;
            else
                buf[i++] = r + 55;
        }
    }

    buf[i++] = 'x';
    buf[i++] = '0';
    buf[i] = '\0';

    return strrev(buf);
}

char* _itoa_bin(int n, char* buf) {
    size_t i = 0;

    if (n == 0) {
        buf[i++] = '0';
    } else {
        for (int x = n; x > 0; x /= 2)
            buf[i++] = x % 2 + 48;
    }

    buf[i] = '\0';

    return strrev(buf);
}

char* itoa(int n, char* buf, char base) {
    switch (base) {
        case 2: return _itoa_bin(n, buf);
        case 10: return _itoa_dec(n, buf);
        case 16: return _itoa_hex(n, buf);
    }

    return NULL;
}
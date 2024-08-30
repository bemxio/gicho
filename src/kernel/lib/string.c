#include "string.h"

size_t strlen(const char* s) {
    size_t i = 0;

    while (s[i] != '\0') {
        i++;
    }

    return i;
}

int strcmp(const char* s1, const char* s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }

    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

char* strtok(char* s, const char* d) {
    static char* p = NULL;

    if (s != NULL) {
        p = s;
    } else if (p == NULL) {
        return NULL;
    }

    s = p;

    size_t l = strlen(d);
    bool f = false;

    while (*p != '\0' && !f) {
        for (size_t i = 0; i < l; i++) {
            if (*p == d[i]) {
                f = true; break;
            }
        }

        p++;
    }

    if (p == s) {
        return NULL;
    } else if (*p != '\0') {
        *(p - 1) = '\0';
    }

    return s;
}
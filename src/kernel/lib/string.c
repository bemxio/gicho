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

char* strrev(char* s) {
    size_t len = strlen(s) - 1;

    for (size_t i = 0; i < len / 2; i++) {
        char temp = s[i];

        s[i] = s[len - i];
        s[len - i] = temp;
    }

    return s;
}

char* strtok(char* s, const char* delim) {
    static char* p = NULL;

    if (s != NULL) {
        p = s;
    } else if (p == NULL) {
        return NULL;
    }

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

    if (p == s) {
        return NULL;
    } else if (*p != '\0') {
        *(p - 1) = '\0';
    }

    return s;
}
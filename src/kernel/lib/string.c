#include "string.h"

void* memset(void* s, int c, size_t n) {
    for (unsigned char* p = (unsigned char*)s; n != 0; n--) {
        *p++ = c;
    }

    return s;
}

void* memcpy(void* restrict s1, const void* restrict s2, size_t n) {
    char* destination = s1;
    const char* source = s2;

    while (n--) {
        *destination++ = *source++;
    }

    return s1;
}

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
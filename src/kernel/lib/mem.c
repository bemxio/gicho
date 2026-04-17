#include "mem.h"

void* memset(void* s, int c, size_t n) {
    for (unsigned char* p = (unsigned char*)s; n != 0; n--)
        *p++ = c;

    return s;
}

void* memcpy(void* restrict s1, const void* restrict s2, size_t n) {
    char* dest = s1;
    const char* src = s2;

    while (n--)
        *dest++ = *src++;

    return s1;
}

void* malloc(size_t size) {
    static void* address = 0x8000; // TODO: calculate this based on kernel size (or implement a proper heap allocator)

    void* ptr = (void*)address;
    address += size;

    return ptr;
}
#include "memory.h"

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
    static unsigned int bytes_allocated = 0;
    void* address = 0x500 + KERNEL_SIZE * 512;

    address += bytes_allocated;
    bytes_allocated += size;

    return address;
}
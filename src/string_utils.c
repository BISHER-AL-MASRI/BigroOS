#include <stddef.h>

void* memset(void* ptr, int value, size_t num) {
    unsigned char* p = ptr;
    while (num--) {
        *p++ = (unsigned char)value;
    }
    return ptr;
}

int strcmp(const char *str1, const char *str2) {
    while (*str1 && (*str1 == *str2)) {
        str1++;
        str2++;
    }
    return *(unsigned char *)str1 - *(unsigned char *)str2;
}


char *strcpy(char *dest, const char *src) {
    char *d = dest;
    while ((*d++ = *src++) != '\0') {}
    return dest;
}

size_t strlen(const char *str) {
    size_t len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
}

int compare_strings(const char *s1, const char *s2) {
    while (*s1 && *s2) {
        if (*s1 != *s2) return 0;
        s1++;
        s2++;
    }
    return *s1 == *s2;
}


int starts_with(const char *str, const char *prefix)
{
    while (*prefix)
    {
        if (*str++ != *prefix++)
        {
            return 0;
        }
    }
    return 1;
}

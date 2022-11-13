#ifndef _STRING_H_
#define _STRING_H_
#include <stdint.h>

#define STRING_MAX_SIZE 65536

uint64_t strlen(const char* str);
void memset(void * ptr, char chr, uint64_t size);

#endif

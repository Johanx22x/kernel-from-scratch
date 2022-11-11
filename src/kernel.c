#include "bootloader/bootservices.h"
#include "print/printf/printf.h"
#include <stdint.h>

#define STRING_MAX_SIZE 65536

uint64_t strlen(const char* str) {
    uint64_t len = 0;
    while (str[len] && len < STRING_MAX_SIZE)
        len++;
    return len;
}

void hlt() {
    while (1) {
        __asm__("hlt");
    }
}

void _start(void) {
    printf("Hi, I'm a kernel!\nBootloader: %s\nBootloader version: %s\n", get_bootloader_name(), get_bootloader_version());
    hlt();
}

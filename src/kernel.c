#include "bootservices.h"
#include <stdint.h>

#define STRING_MAX_SIZE 65536

uint64_t strlen(const char* str) {
    uint64_t len = 0;
    while (str[len] && len < STRING_MAX_SIZE)
        len++;
    return len;
}

void print(const char * str) {
    void (*writer)(const char *, uint64_t) = get_terminal_writer();
    writer(str, strlen(str));
}

void hlt() {
    while (1) {
        __asm__("hlt");
    }
}

void _start(void) {
    print("Welcome to the kernel!\n");
    print("This is a kernel written in C.\n");
    print("It uses ");
    print(get_bootloader_name());
    print(" version ");
    print(get_bootloader_version());
    print(" as a bootloader.\n");
    hlt();
}

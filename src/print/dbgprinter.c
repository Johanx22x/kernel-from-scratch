#include "dbgprinter.h"
#include "./printf/printf.h"

void panic(const char *msg) {
    printf("PANIC: %s\n", msg);
    while (1) {
        __asm__("hlt");
    }
}

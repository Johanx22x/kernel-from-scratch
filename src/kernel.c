#include "bootloader/bootservices.h"
#include "io/interrupts.h"
#include "print/printf/printf.h"
#include "print/dbgprinter.h"
#include "util/string.h"
#include "memory/memory.h"
#include <stdint.h>

void hlt() {
    while (1) {
        __asm__("hlt");
    }
}

void _start(void) {
    printf("Hi, I'm a kernel!\nBootloader: %s\nBootloader version: %s\n", get_bootloader_name(), get_bootloader_version());

    init_memory();
    init_interrupts();

    uint64_t * badptr = (uint64_t*)0xffffffffdeadb000;
    *badptr = 0xdeadbeef;

    panic("Kernel looping...");
}

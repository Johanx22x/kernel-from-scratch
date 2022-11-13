#include "bootloader/bootservices.h"
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

    void * page = request_page();
    printf("Allocated page at %p\n", page);

    void * page2 = request_page();
    printf("Allocated page at %p\n", page2);

    free_page(page2);

    void * page3 = request_page();
    printf("Allocated page at %p\n", page3);

    panic("Kernel looping...");
}

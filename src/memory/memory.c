#include "memory.h"
#include "../bootloader/bootservices.h"
#include "../print/printf/printf.h"
#include "../print/dbgprinter.h"
#include "../util/string.h"
#include <stdint.h>

#define ADDR_TO_PAGE(addr) ((uint64_t)(addr) >> 12)

struct uefi_memory_region working_memory = {
    .type = LIMINE_MEMMAP_BAD_MEMORY,
    .length = 0,
    .base = 0
};

uint64_t last_requested;
uint64_t total_pages;
uint8_t *physical_region;

void lock_page(uint64_t page) {
    physical_region[page] = 1;
    last_requested = page;
}

void unlock_page(uint64_t page) {
    printf("Unlocking page %d\n", page);
    physical_region[page] = 0;
}

void free_page(void * addr) {
    uint64_t addr_uint = (uint64_t)addr;
    uint64_t addr_normalized = (addr_uint - working_memory.base);

    if (addr_uint < working_memory.base) {
        panic("Attempted to free page outside of working memory!");
    }

    if (physical_region[ADDR_TO_PAGE(addr_normalized)] == 0) {
        panic("Attempted to free page that was not locked!");
    }

    unlock_page(ADDR_TO_PAGE(addr_normalized));

    if (physical_region[ADDR_TO_PAGE(addr_normalized)] == 1) {
        panic("Failed to free page!");
    }
}

void * request_page() {
    uint64_t first = last_requested - 1;

    while (physical_region[last_requested]) {
        if (last_requested >= total_pages) {
            last_requested = 0;
        } if (last_requested == first) {
            panic("Out of memory and no swap available"); // TODO: Implement swap
        }
        last_requested++;
    }

    lock_page(last_requested);

    if (physical_region[last_requested] == 0) {
        panic("Page was not locked");
    }

    return (void *)(working_memory.base + (last_requested * 0x1000));
}

void init_pfa() { // pfa = Page Frame Allocator
    total_pages = working_memory.length / 0x1000; // 4096 bytes
    physical_region = (uint8_t *)working_memory.base;

    memset((void *)physical_region, 0, total_pages);

    for (uint64_t i = 0; i < (total_pages / 0x1000); i++) {
        lock_page(i);
    }
}

void init_memory() {
    printf("Loading memory...\n");

    uint64_t entries = get_memory_map_entries();

    printf("Memory map entries: %d\n", entries);

    uint64_t total_memory = 0;
    uint64_t total_usable_memory = 0;

    for (uint64_t i = 0; i < entries; i++) {
        uint64_t type = get_memory_map_type(i);
        uint64_t base = get_memory_map_base(i);
        uint64_t length = get_memory_map_length(i);

        printf("Memory region %d, base: %d, type: %d, length: %d\n", i, base, type, length);

        total_memory += length;

        if (type != LIMINE_MEMMAP_USABLE) {
            continue;
        }

        total_usable_memory += length;

        if (length <= working_memory.length) {
            continue;
        }

        working_memory.type = type;
        working_memory.base = base;
        working_memory.length = length;
    }

    printf("Total memory: %d\n", total_memory);
    printf("Total usable memory: %d\n", total_usable_memory);
    printf("Working memory region, base: %d, type: %d, length: %d\n", working_memory.base, working_memory.type, working_memory.length);

    init_pfa();
}

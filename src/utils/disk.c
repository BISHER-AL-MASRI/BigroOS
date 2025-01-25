#include "disk.h"
#include "vga.h"
#include "utils/util.h"

int _read_from_disk(uint32_t sector, void* buffer, uint32_t size) {
    // s = sector, b = buffer, n = size
    outportb(0x1F7, 0xE);  // command: read sectors
    outportb(0x1F0, sector & 0xFF);
    outportb(0x1F1, (sector >> 8) & 0xFF);
    outportb(0x1F2, (sector >> 16) & 0xFF);
    outportb(0x1F3, (sector >> 24) & 0xFF);
    outportb(0x1F4, 0x20);  // number of sectors to read
    outportb(0x1F5, size & 0xFF);
    outportb(0x1F6, (size >> 8) & 0xFF);
    outportb(0x1F7, 0x20);  // read sectors
    
    for (uint32_t i = 0; i < size; i++) {
        ((uint8_t*) buffer)[i] = inportb(0x1F7);
    }
    return 0;
}

int _write_to_disk(uint32_t sector, const void* buffer, uint32_t size) {
    // s = sector, b = buffer, n = size
    outportb(0x1F7, 0xE);  // command: write sectors
    outportb(0x1F0, sector & 0xFF);
    outportb(0x1F1, (sector >> 8) & 0xFF);
    outportb(0x1F2, (sector >> 16) & 0xFF);
    outportb(0x1F3, (sector >> 24) & 0xFF);
    outportb(0x1F4, 0x20);  // number of sectors to write
    outportb(0x1F5, size & 0xFF);
    outportb(0x1F6, (size >> 8) & 0xFF);
    outportb(0x1F7, 0x30);  // write sectors
    
    for (uint32_t i = 0; i < size; i++) {
        outportb(0x1F7, ((uint8_t*) buffer)[i]);
    }
    return 0;
}
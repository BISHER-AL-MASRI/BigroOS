#include "ext2.h"
#include "vga.h" 

#define EXT2_FS_BASE_ADDRESS 0x100000  // Example: Base address of ext2 filesystem in memory

static inline uint8_t inb(uint16_t port) {
    uint8_t result;
    asm volatile("inb %1, %0" : "=a"(result) : "Nd"(port));
    return result;
}
static inline void outb(uint16_t port, uint8_t value) {
    asm volatile("outb %0, %1" : : "a"(value), "Nd"(port));
}

void read_superblock() {
    ext2_super_block sb;
    read_from_disk(EXT2_FS_BASE_ADDRESS + 1024, &sb, sizeof(ext2_super_block));
    uint32_t block_size = 1024 << sb.s_log_block_size;

    terminal_writestring("Block size: ");
    terminal_writeint(block_size);
    terminal_writestring("\n");
}

uint32_t get_block_address_from_inode(uint32_t inode) {
    ext2_super_block sb;
    read_from_disk(EXT2_FS_BASE_ADDRESS + 1024, &sb, sizeof(ext2_super_block));
    uint32_t block_size = 1024 << sb.s_log_block_size;
    return sb.s_first_data_block + (inode - 1) / sb.s_blocks_count * sb.s_blocks_count;
}

// Assuming you already have a function to read the inode table and file blocks
void ext2_list_directory(uint32_t inode_address) {
    char buffer[1024];  // assuming 1024-byte blocks for simplicity
    uint32_t block_address = get_block_address_from_inode(inode_address);
    
    read_from_disk(block_address, buffer, sizeof(buffer));

    uint32_t offset = 0;
    while (offset < 1024) {
        ext2_dir_entry *entry = (ext2_dir_entry *)(buffer + offset);
        if (entry->inode == 0) break;

        terminal_writestring(entry->name);
        terminal_writestring("\n");

        offset += entry->rec_len;
    }
}

// Corrected inode block calculation for reading files

uint32_t get_inode_for_file(const char *filename) {
    ext2_super_block sb;
    read_from_disk(EXT2_FS_BASE_ADDRESS + 1024, &sb, sizeof(ext2_super_block));
    uint32_t block_size = 1024 << sb.s_log_block_size;

    uint32_t inode = 0;
    for (int i = 0; i < sb.s_inodes_count; i++) {
        ext2_dir_entry entry;
        read_from_disk(EXT2_FS_BASE_ADDRESS + 1024 + i * sizeof(ext2_dir_entry), &entry, sizeof(ext2_dir_entry));
        if (compare_strings(entry.name, filename)) {
            inode = entry.inode;
            break;
        }
    }
    return inode;
}

int ext2_read_file(const char *filename, char *buffer, uint32_t buffer_size) {
    ext2_super_block sb;
    read_from_disk(EXT2_FS_BASE_ADDRESS + 1024, &sb, sizeof(ext2_super_block));
    uint32_t block_size = 1024 << sb.s_log_block_size;

    uint32_t inode = get_inode_for_file(filename);  // Implement function to search for inode

    if (inode == 0) {
        terminal_writestring("File not found.\n");
        return 1;
    }

    // Get the inode and read its block addresses
    uint32_t block_address = get_block_address_from_inode(inode);

    uint32_t offset = 0;
    while (offset < buffer_size) {
        read_from_disk(block_address + offset, buffer + offset, block_size);
        offset += block_size;
    }

    return 0;
}

void read_from_disk(uint32_t sector, void *buffer, uint32_t size) {
    __asm__ __volatile__(
        "movl %[sector], %%ecx;"
        "movl %[buffer], %%esi;"
        "movl %[size], %%edx;"
        "call _read_from_disk;"
        :
        : [sector] "m"(sector), [buffer] "m"(buffer), [size] "m"(size)
        : "%ecx", "%esi", "%edx"
    );
}

void write_to_disk(uint32_t sector, const void *buffer, uint32_t size) {
    __asm__ __volatile__(
        "movl %[sector], %%ecx;"
        "movl %[buffer], %%esi;"
        "movl %[size], %%edx;"
        "call _write_to_disk;"
        :
        : [sector] "m"(sector), [buffer] "m"(buffer), [size] "m"(size)
        : "%ecx", "%esi", "%edx"
    );
}
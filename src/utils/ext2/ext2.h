#ifndef EXT2_H
#define EXT2_H

#include <stdint.h>

typedef struct {
    uint32_t inode;
    uint16_t rec_len;
    uint8_t name_len;
    uint8_t file_type;
    char name[];
} ext2_dir_entry;

typedef struct {
    uint32_t s_inodes_count;
    uint32_t s_blocks_count;
    uint32_t s_r_blocks_count;
    uint32_t s_free_blocks_count;
    uint32_t s_free_inodes_count;
    uint32_t s_first_data_block;
    uint32_t s_log_block_size;
    // Other fields...
} ext2_super_block;

uint32_t get_block_address_from_inode(uint32_t inode);
int ext2_read_file(const char *filename, char *buffer, uint32_t buffer_size);
void ext2_list_directory(uint32_t block_address);
uint32_t get_inode_for_file(const char *filename);
extern void read_from_disk(uint32_t sector, void *buffer, uint32_t size);
extern void write_to_disk(uint32_t sector, const void *buffer, uint32_t size);



#endif

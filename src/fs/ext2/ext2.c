#include "ext2.h"
#include "../../utils/util.h"
#include "vga.h"
#include "../../utils/disk.h"

static struct ext2_superblock sb;
static uint8_t block_buffer[EXT2_BLOCK_SIZE];

// Helper function to read a block from disk
static int read_block(uint32_t block_num) {
    // Calculate sector number (1 block = 2 sectors as block size is 1024 and sector size is 512)
    uint32_t sector = block_num * 2;
    return _read_from_disk(sector, block_buffer, EXT2_BLOCK_SIZE);
}

int ext2_init(void) {
    // Read superblock (located at block 1)
    if (read_block(1) != 0) {
        return -1;
    }
    
    // Copy superblock from buffer
    memcpy(&sb, block_buffer, sizeof(struct ext2_superblock));
    
    if (sb.s_magic != EXT2_SUPER_MAGIC) {
        return -1;
    }
    
    return 0;
}

int ext2_read_inode(uint32_t inode_num, struct ext2_inode *inode) {
    uint32_t block_group = (inode_num - 1) / sb.s_inodes_per_group;
    uint32_t index = (inode_num - 1) % sb.s_inodes_per_group;
    uint32_t block = sb.s_first_data_block + 
                     block_group * sb.s_blocks_per_group + 
                     (index * sizeof(struct ext2_inode)) / EXT2_BLOCK_SIZE;
    
    if (read_block(block) != 0) {
        return -1;
    }
    
    uint32_t offset = (index * sizeof(struct ext2_inode)) % EXT2_BLOCK_SIZE;
    memcpy(inode, block_buffer + offset, sizeof(struct ext2_inode));
    return 0;
}

int ext2_read_directory(uint32_t inode_num) {
    struct ext2_inode inode;
    if (ext2_read_inode(inode_num, &inode) != 0) {
        return -1;
    }
    
    // Read direct blocks
    for (int i = 0; i < 12 && inode.i_block[i]; i++) {
        if (read_block(inode.i_block[i]) != 0) {
            return -1;
        }
        
        uint32_t offset = 0;
        while (offset < EXT2_BLOCK_SIZE) {
            struct ext2_dir_entry *entry = (struct ext2_dir_entry *)(block_buffer + offset);
            if (entry->inode == 0) {
                break;
            }
            
            // Print directory entry (for testing)
            char name[EXT2_NAME_LEN + 1];
            memcpy(name, entry->name, entry->name_len);
            name[entry->name_len] = '\0';
            terminal_writestring(name);
            terminal_writestring("\n");
            
            offset += entry->rec_len;
        }
    }
    
    return 0;
}


#ifndef DISK_H
#define DISK_H

#include <stdint.h>

// Read from disk
// sector: sector number to read from
// buffer: buffer to read into
// size: number of bytes to read
extern int _read_from_disk(uint32_t sector, void* buffer, uint32_t size);

// Write to disk
// sector: sector number to write to
// buffer: buffer to write from
// size: number of bytes to write
extern int _write_to_disk(uint32_t sector, const void* buffer, uint32_t size);

#endif
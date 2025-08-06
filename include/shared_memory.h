//
// Created by Bib's on 05-08-25.
//

#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

#include <stdbool.h>

// Attach a shared memory block
// Associated with filename
// Create it if it doesn't exist
char *attach_memory_block(char *filename, int size);

// Detach the memory block from the process
bool detach_memory_block(char *block);

// Destroy the shared memory block
bool destroy_memory_block(char *filename);

// All of the programs will share these values
#define BLOCK_SIZE 4096



#endif //SHARED_MEMORY_H

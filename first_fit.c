#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_BLOCKS 100

// Structure to represent a memory block
typedef struct MemoryBlock
{
    int id;           // Block identifier
    int size;         // Size of the block
    int is_allocated; // Flag to check if block is allocated
} MemoryBlock;

// Global array to store memory blocks
MemoryBlock memory_blocks[MAX_BLOCKS];
int total_blocks = 0;

// Function to initialize memory blocks
void initialize_memory()
{
    for (int i = 0; i < MAX_BLOCKS; i++)
    {
        memory_blocks[i].id = -1;
        memory_blocks[i].size = 0;
        memory_blocks[i].is_allocated = 0;
    }
}

// Function to add a memory block
void add_memory_block(int size)
{
    if (total_blocks >= MAX_BLOCKS)
    {
        printf("Memory pool is full. Cannot add more blocks.\n");
        return;
    }

    memory_blocks[total_blocks].id = total_blocks;
    memory_blocks[total_blocks].size = size;
    memory_blocks[total_blocks].is_allocated = 0;
    total_blocks++;
}

// First Fit memory allocation algorithm
int allocate_memory(int request_size)
{
    // Iterate through memory blocks
    for (int i = 0; i < total_blocks; i++)
    {
        // Check if block is free and has enough size
        if (!memory_blocks[i].is_allocated && memory_blocks[i].size >= request_size)
        {
            // Allocate the first suitable block found
            memory_blocks[i].is_allocated = 1;

            printf("Memory allocated: Block ID %d, Size %d\n",
                   memory_blocks[i].id,
                   memory_blocks[i].size);

            return i;
        }
    }

    printf("No suitable memory block found for size %d\n", request_size);
    return -1;
}

// Function to free allocated memory
void free_memory(int block_id)
{
    if (block_id >= 0 && block_id < total_blocks)
    {
        if (memory_blocks[block_id].is_allocated)
        {
            memory_blocks[block_id].is_allocated = 0;
            printf("Memory freed: Block ID %d, Size %d\n",
                   memory_blocks[block_id].id,
                   memory_blocks[block_id].size);
        }
        else
        {
            printf("Block %d is already free\n", block_id);
        }
    }
    else
    {
        printf("Invalid block ID\n");
    }
}

// Function to display memory blocks
void display_memory_status()
{
    printf("\nMemory Block Status:\n");
    printf("---------------------\n");
    for (int i = 0; i < total_blocks; i++)
    {
        printf("Block %d: Size = %d, Status = %s\n",
               memory_blocks[i].id,
               memory_blocks[i].size,
               memory_blocks[i].is_allocated ? "Allocated" : "Free");
    }
    printf("\n");
}

// Main function to demonstrate memory allocation
int main()
{
    // Initialize memory
    initialize_memory();

    // Add memory blocks to the pool
    add_memory_block(100);
    add_memory_block(500);
    add_memory_block(200);
    add_memory_block(300);
    add_memory_block(600);

    // Display initial memory status
    display_memory_status();

    // Allocate memory using First Fit algorithm
    int block1 = allocate_memory(250);
    int block2 = allocate_memory(150);
    int block3 = allocate_memory(400);

    // Display memory status after allocation
    display_memory_status();

    // Free some allocated memory
    free_memory(block1);
    free_memory(block2);

    // Display final memory status
    display_memory_status();

    return 0;
}
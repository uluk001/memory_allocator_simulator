# include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


struct MemoryBlock {
    size_t size;
    bool is_free;
    struct MemoryBlock* bk;
    struct MemoryBlock* fd;
};

struct MemoryBlock* heap_start = NULL;
size_t min_size = 8;


void initialize_heap(size_t size) {
    heap_start = malloc(sizeof(struct MemoryBlock) + size);
    heap_start -> size = size;
    heap_start -> is_free = true;
    heap_start -> bk = NULL;
    heap_start -> fd = NULL;
}



struct MemoryBlock* find_free_block(struct MemoryBlock* heap, size_t size) {
    struct MemoryBlock* current_chunk = heap;
    while (current_chunk != NULL)
    {
        if (current_chunk -> is_free && current_chunk -> size >= size) {

            return current_chunk;
        };

        current_chunk = current_chunk -> fd;
    };

    return NULL;    
}



struct MemoryBlock* split_block(struct MemoryBlock* chunk, size_t size) {

    if (chunk -> size < sizeof(struct MemoryBlock) + size + min_size) {
        return chunk;
    };

    struct MemoryBlock* new_chunk = (struct MemoryBlock*)((char*)(chunk + 1) + size);

    new_chunk -> size = chunk -> size - size - sizeof(struct MemoryBlock);
    new_chunk -> is_free = true;
    new_chunk -> fd = chunk -> fd;
    new_chunk -> bk = chunk;

    chunk -> size = size;
    chunk -> fd = new_chunk;

    if (new_chunk -> fd != NULL) {
        new_chunk -> fd -> bk = new_chunk;
    };

    return chunk;
}


void* custom_malloc(size_t size) {
    struct MemoryBlock* new_chunk = find_free_block(heap_start, size);
    printf("custom_malloc works!!!: %zu\n", heap_start -> size);

    if (new_chunk != NULL) {
        new_chunk = split_block(new_chunk, size);
        new_chunk -> is_free = false;
        return (void*)(new_chunk + 1);
    };
    return NULL;
}


int main()
{
    initialize_heap(1000);
    printf("Size: %zu\n", heap_start -> size);
    printf("Is free: %d\n", heap_start -> is_free);
    printf("Located: %p\n\n", heap_start);

    void* ptr1 = custom_malloc(100);
    void* ptr2 = custom_malloc(200);
    void* ptr3 = custom_malloc(1001);

    struct MemoryBlock* current = heap_start;
    printf("\nMemory blocks state:\n");
    int i = 0;
    while (current != NULL) {
        printf("Block %d at %p:\n", i++, current);
        printf("  Size: %zu\n", current->size);
        printf("  Is free: %d\n", current->is_free);
        printf("  Next: %p\n", current->fd);
        printf("  Prev: %p\n\n", current->bk);
        current = current->fd;
    }
}

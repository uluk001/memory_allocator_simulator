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


// Ищет свободного блока памяти подходящего размера
struct MemoryBlock* find_free_block(struct MemoryBlock* heap, size_t size) {
    struct MemoryBlock* current_chunk = heap;
    // Проверяем каждый следующий блока до тех пор пока есть блока
    while (current_chunk != NULL)
    {
        // свободный ли chunk и его размер больше ли запращиваемого
        if (current_chunk -> is_free && current_chunk -> size >= size) {
            // если да возвращаем
            return current_chunk;
        };
        // если нет переходим к следующей
        current_chunk = current_chunk -> fd;
    };
    // если не нашли возвращаем NULL
    return NULL;    
}


// Создает новый блок путем разделения существующей
struct MemoryBlock* split_block(struct MemoryBlock* chunk, size_t size) {
    // Проверяем имеет ли смысл раделять. Чтобы можно было, он должен иметь достаточно большой размер чтобы он оставался больше после вычета всех расходов на создание нового блока
    // sizeof(struct MemoryBlock) - это для метаданных нового блока, которую мы собираемся создавать
    // size - это размер которую мы запрашиваем
    // min_size - это минимальный размер которая выделяется
    // Если суммарно параметры выше больше чем размер текущего chunka то просто нету смысла разделять текущий блок.
    // Пример: если размер текущего блока = 19 байт, размер структуры метаданных = 4 байт,
    // минимальный размер блока = 8 байт, и запрашиваемый размер = 8 байт,
    // то: 19 < (4 + 8 + 8) => разделять блок нельзя, так как не хватит места
    // для создания жизнеспособного второго блока
    if (chunk -> size < sizeof(struct MemoryBlock) + size + min_size) {
        return chunk;
    };

    // Вычисляем адрес для метаданных нового блока памяти
    // chunk + 1 - переходим за метаданные текущего блока
    // (char*) - преобразуем в побайтовый указатель для корректной адресации
    // + size - пропускаем область данных, которую выделяем пользователю
    // И в итоге указатель будет указывать на место, где начнутся метаданные нового блока
    struct MemoryBlock* new_chunk = (struct MemoryBlock*)((char*)(chunk + 1) + size);

    // Вычисляем размер для нового блока, все что останется из изначального размера после вычета
    // запращиваемого размера и размера которую занимают метаданные
    new_chunk -> size = chunk -> size - size - sizeof(struct MemoryBlock);
    // ставим флаг что новый блок свободный
    new_chunk -> is_free = true;
    // как указателб на следующий блок ставим указатель на следующий блок из старого (fd - forward)
    new_chunk -> fd = chunk -> fd;
    // как предыдущий ставим указатель на старый блок
    new_chunk -> bk = chunk;

    // у старого размер ставим тот что запрашивал пользователь
    chunk -> size = size;
    // как указатель на следующий ставим new_chunk
    chunk -> fd = new_chunk;

    // Если у нового блока есть указатель на следующий блок, то ему меняем fd (указатель на пред блок) на новый
    if (new_chunk -> fd != NULL) {
        new_chunk -> fd -> bk = new_chunk;
    };

    // возвращаем старый блок
    return chunk;
}


void* custom_malloc(size_t size) {
    // Ищет свободный блок
    struct MemoryBlock* new_chunk = find_free_block(heap_start, size);
    printf("custom_malloc works!!!: %zu\n", heap_start -> size);

    // Если есть подходящий возвращает его иначе NULL
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

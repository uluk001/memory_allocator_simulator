# Custom Memory Allocator Implementation

A basic memory allocator in C that demonstrates memory management using custom malloc and free implementations.

## Installation

1. Make sure you have a C compiler (gcc) installed
2. Clone the repository:
```bash
git clone https://github.com/uluk001/memory_allocator_simulator.git
```
3. Compile and run:
```bash
gcc -o memory_allocator main.c
./memory_allocator
```

## Core Features

The allocator provides:
- Custom memory allocation (`custom_malloc`)
- Memory deallocation (`custom_free`) 
- Memory block splitting to reduce fragmentation
- Block coalescing during deallocation

## Usage

```c
// Initialize heap
initialize_heap(1000);

// Allocate memory
void* ptr = custom_malloc(100);

// Free memory
custom_free(ptr);
```

## Structure

The allocator uses a linked list of memory blocks, with each block containing metadata about its size, allocation status, and pointers to adjacent blocks.

## Limitations

- Basic first-fit allocation strategy
- No memory alignment
- Limited error handling
- No thread safety

## Contributing

Feel free to fork the repository and submit pull requests. Areas for improvement include better allocation strategies, memory alignment, and thread safety.

## Author

[GitHub](https://github.com/uluk001)  
[LinkedIn](https://www.linkedin.com/in/ismailov-uluk-92784a233/)

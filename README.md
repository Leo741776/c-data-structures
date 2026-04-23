# Lightweight Data Structures Library
This is a data structures library for C. It includes a collection of data structures (stack, queue, linked list, hash table, max-heap). I made this mainly for learning purposes.

## How to Run
- build: `make`
- run: `./ldsl_demo`
- clean: `make clean`

## Overview
- **Stack:** Dynamic array, LIFO order. top serves as both element count and next-insert index. Doubles capacity via realloc when full.
- **Queue:** Circular buffer, FIFO order. head/tail indices wrap around using modulo — avoids shifting elements on dequeue. Doubles capacity and unwraps the circular layout on resize.
- **Linked List:** Singly linked with head and tail pointers. tail enables O(1) tail insertion; removal is still O(n) since you can't walk backwards. Internal ll_at() helper traverses to any index.
- **Hash Table:** Separate chaining (each bucket is a linked list of Entry nodes). djb2 hashing algorithm for strings. Rehashes (doubles + redistributes) when load factor exceeds 0.7. Keys are strdup'd so the table owns its own memory.
- **Max Heap:** Array-based binary max-heap. Insert sifts up; remove (pop max) replaces root with last element and sifts down. Child/parent indices use standard 0-indexed formulas: left = 2i+1, parent = (i-1)/2.

## Design
- All functions take a pointer to their structure as the first argument.
- Functions return int (0 = success, -1 = failure) or the value type (double / pointer).
- NAN is returned for invalid reads (empty stack, key not found, etc.).
- Each structure has a \_create() and a matching \_free() for full memory management.
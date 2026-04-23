CC = gcc
CFLAGS = -Wall -Wextra -Iinclude

SRCS = src/stack.c src/queue.c src/linked_list.c src/hash_table.c src/heap.c demo/main.c

run: $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o run

clean:
	rm -f run
CC = gcc
CFLAGS = -Wall -Wextra -Iinclude

SRCS = src/stack.c src/queue.c src/linked_list.c src/hash_table.c src/heap.c demo/main.c

demo: $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o ldsl_demo

clean:
	rm -f ldsl_demo
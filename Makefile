CC = gcc
CFLAGS = -Wall -Wextra -pthread -Iinclude

EXAMPLE = examples/example_threadpool
TEST = test/test_threadpool

all: example test

example:
	gcc $(CFLAGS) examples/example_threadpool.c src/threadpool.c -o $(EXAMPLE)

test:
	gcc $(CFLAGS) test/test_threadpool.c src/threadpool.c -o $(TEST)

clean:
	rm -f $(EXAMPLE) $(TEST)

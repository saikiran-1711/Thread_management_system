CC = gcc
CFLAGS = -Wall -Wextra -pthread -Iinclude

EXAMPLE_BIN = examples/example_threadpool
TEST_BIN = test/test_threadpool

all: example test

example:
	$(CC) $(CFLAGS) examples/example_threadpool.c src/threadpool.c -o $(EXAMPLE_BIN)

test:
	$(CC) $(CFLAGS) test/test_threadpool.c src/threadpool.c -o $(TEST_BIN)

clean:
	rm -f $(EXAMPLE_BIN) $(TEST_BIN)

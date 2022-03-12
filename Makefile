CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -Waggregate-return -Wwrite-strings -Wvla -Wfloat-equal
INCLUDE = -I ../include/

.PHONY: all
all: objects dstruct test

test:
	$(CC) $(CFLAGS) $(INCLUDE) -o test ./obj/test_main.o ./obj/thread_pool.o ./obj/circular_ll.o ./obj/queue.o -pthread

objects:
	$(MAKE) -C ./src/

dstruct:
	$(MAKE) -C ./dstructs/

.PHONY: debug
debug: CFLAGS += -g
debug: clean
debug: all

.PHONY: clean
clean:
	$(RM) ./obj/*.o test
CFLAGS = -Wall -Wextra -Wpedantic -Waggregate-return -Wwrite-strings -Wvla -Wfloat-equal
INCLUDE = -I../include/ -I../thread_pool/ -I../dstructs/
export CFLAGS
export INCLUDE

.PHONY: all
all: src dstruct thread_pool test

test:
	$(CC) $(CFLAGS) $(INCLUDE) -o test $(wildcard ./obj/*.o) -pthread

.PHONY: src
src:
	$(MAKE) -C ./src/

.PHONY: dstruct
dstruct:
	$(MAKE) -C ./dstructs/

.PHONY: thread_pool
thread_pool:
	$(MAKE) -C ./thread_pool/

.PHONY: debug
debug: CFLAGS += -g
debug: clean
debug: all

.PHONY: clean
clean:
	$(RM) ./obj/*.o test
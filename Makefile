sources := $(wildcard *.c)
objects := $(patsubst %.c, %.o, $(sources))

.PHONY: all
all: $(objects) stack queue

.PHONY: stack
stack:
	$(MAKE) -C stack/

.PHONY: queue
queue:
	$(MAKE) -C queue/

$(objects): %.o: %.c

%.o:
	$(CC) $(CFLAGS) $(INCLUDE) -o ../obj/$@ -c $^

sources := $(wildcard *.c)
objects := $(patsubst %.c, %.o, $(sources))
INCLUDE += -I./queue/

.PHONY: all
all: $(objects) queue

.PHONY: queue
queue:
	$(MAKE) -C queue/

$(objects): %.o: %.c

%.o:
	$(CC) $(CFLAGS) $(INCLUDE) -o $(obj_path)$@ -c $^

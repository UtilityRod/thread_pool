sources := $(wildcard *.c)
objects := $(patsubst %.c, %.o, $(sources))

.PHONY: all
all: $(objects)

$(objects): %.o: %.c

%.o:
	$(CC) $(CFLAGS) $(INCLUDE) -o ../obj/$@ -c $^
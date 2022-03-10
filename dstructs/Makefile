CFLAGS = -Wall -Wextra -Wpedantic -Waggregate-return -Wwrite-strings -Wvla -Wfloat-equal
INCLUDE = -I ../include/
sources := $(wildcard *.c)
objects := $(patsubst %.c, %.o, $(sources))

.PHONY: all
all: $(objects)

$(objects): %.o: %.c

%.o:
	$(CC) $(CFLAGS) $(INCLUDE) -o ../obj/$@ -c $^
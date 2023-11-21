CC = gcc
CFLAGS = -pthread

SRCS = dining_philosophers.c

all: dining_philosophers

dining_philosophers: $(SRCS)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f dining_philosophers

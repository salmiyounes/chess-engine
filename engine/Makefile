CC = gcc

CFLAGS = -Wall -Wextra -std=c11 -fPIC

SRCS = bb.c board.c gen.c move.c

HEADERS = bb.h board.h gen.h move.h

OBJS = $(SRCS:.c=.o)

TARGET = libchess.so

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -shared -o $@ $(OBJS)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

clean-precompiled:
	rm -f *.gch

clean-all: clean clean-precompiled

.PHONY: all clean clean-precompiled clean-all

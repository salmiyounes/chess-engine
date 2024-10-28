CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -fPIC -O3 
SRCS = bb.c board.c gen.c move.c perft.c search.c table.c eval.c
HEADERS = bb.h board.h gen.h move.h perft.h search.h table.h eval.h
OBJS = $(SRCS:.c=.o)
DEPS = $(SRCS:.c=.d)
TARGET = libchess.so.1

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -shared -Wl,-soname,libchess.so.1 -o $@ $(OBJS)
	ln -sf $@ libchess.so

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

-include $(DEPS)

clean:
	rm -f $(OBJS) $(TARGET) libchess.so

clean-precompiled:
	rm -f *.gch

clean-deps:
	rm -f $(DEPS)

clean-all: clean clean-precompiled clean-deps

.PHONY: all clean clean-precompiled clean-deps clean-all

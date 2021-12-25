EXEC = darksky
SRCS = $(wildcard *.c)

CC = gcc
CFLAGS = -c -g -std=c99 -Wall -Wextra -Wpedantic
LDIR =
LFLAGS = -flto -lcurses

OBJS = $(SRCS:.c=.o)

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDIR) $(LFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

# $@ is the left side of the :
# $^ is the right side of the :
# the $< is the first item in the dependencies list
# $@: $< $^

.PHONY: clean oclean release

clean: oclean
	rm -f $(EXEC)

oclean:
	rm -f *.[oO]

release: all oclean


CC = clang
CFLAGS = -Wall -Wextra -Werror -Wpedantic
LDFLAGS = -lm
EXEC = sorting
OBJS = sorting.o batcher.o heap.o quick.o shell.o set.o stats.o

all: clean $(EXEC) format

$(EXEC): $(OBJS)
	$(CC) $(LDFLAGS) -o $(EXEC) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(EXEC) $(OBJS)

format:
	clang-format -i -style=file *.[ch]
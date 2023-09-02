CC = clang
CFLAGS = -Wall -Wextra -Werror -Wpedantic
LDFLAGS = -lncurses
EXEC = life
OBJS = universe.o life.o

all: clean $(EXEC) format

$(EXEC): $(OBJS)
	$(CC) -o $(EXEC) $(OBJS) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(EXEC) $(OBJS)

format:
	clang-format -i -style=file *.[ch]
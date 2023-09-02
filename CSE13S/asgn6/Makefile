CC = clang
CFLAGS = -Wall -Wextra -Werror -Wpedantic
EXEC = encode decode
ALL_OBJS = encode.o decode.o trie.o word.o io.o
OBJS = trie.o word.o io.o
SOURCES = trie.c word.c io.c

all: clean $(EXEC) format

encode: encode.o $(OBJS)
	$(CC) -o encode encode.o $(OBJS)

decode: decode.o $(OBJS)
	$(CC) -o decode decode.o $(OBJS)

encode.o: encode.c $(SOURCES)
	$(CC) $(CFLAGS) -c encode.c $(SOURCES)

decode.o: decode.c $(SOURCES)
	$(CC) $(CFLAGS) -c decode.c $(SOURCES)

trie.o: trie.c
	$(CC) $(CFLAGS) -c trie.c

word.o: word.c
	$(CC) $(CFLAGS) -c word.c

io.o: io.c
	$(CC) $(CFLAGS) -c io.c

format:
	clang-format -i -style=file *.[ch]

clean:
	rm -f $(EXEC) $(ALL_OBJS)

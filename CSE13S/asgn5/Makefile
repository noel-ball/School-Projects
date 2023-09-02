CC = clang
GMP_CFLAGS = $(shell pkg-config --cflags gmp)
GMP_LFLAGS = $(shell pkg-config --libs gmp)
CFLAGS = $(GMP_CFLAGS) -Wno-format-invalid-specifier -Wno-format-extra-args -Wno-format
LDFLAGS = $(GMP_LFLAGS)
EXEC = keygen encrypt decrypt
ALL_OBJS = keygen.o encrypt.o decrypt.o ss.o numtheory.o randstate.o
OBJS = ss.o numtheory.o randstate.o


all: clean $(EXEC) format

keygen: keygen.o $(OBJS)
	$(CC) -o keygen keygen.o $(OBJS) $(LDFLAGS)

encrypt: encrypt.o $(OBJS)
	$(CC) -o encrypt encrypt.o $(OBJS) $(LDFLAGS)

decrypt: decrypt.o $(OBJS)
	$(CC) -o decrypt decrypt.o $(OBJS) $(LDFLAGS)

keygen.o: keygen.c ss.c numtheory.c randstate.c
	$(CC) $(CFLAGS) -c keygen.c ss.c numtheory.c randstate.c

encrypt.o: encrypt.c ss.c numtheory.c randstate.c
	$(CC) $(CFLAGS) -c encrypt.c ss.c numtheory.c randstate.c

decrypt.o: decrypt.c ss.c numtheory.c randstate.c
	$(CC) $(CFLAGS) -c decrypt.c ss.c numtheory.c randstate.c

ss.o: ss.c numtheory.c randstate.c
	$(CC) $(CFLAGS) -c ss.c numtheory.c randstate.c

numtheory.o: numtheory.c ss.c randstate.c
	$(CC) $(CFLAGS) -c numtheory.c ss.c randstate.c

randstate.o: randstate.c ss.c numtheory.c
	$(CC) $(CFLAGS) -c randstate.c ss.c numtheory.c

clean:
	rm -f $(EXEC) $(ALL_OBJS)

format:
	clang-format -i -style=file *.[ch]

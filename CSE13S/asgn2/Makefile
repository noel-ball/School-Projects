CC = clang
CFLAGS = -Wall -Wextra -Werror -Wpedantic
LDFLAGS = -lm
EXEC = mathlib-test
OBJS = mathlib-test.o bbp.o e.o euler.o madhava.o newton.o viete.o

all: clean $(EXEC) format

$(EXEC): $(OBJS)
	$(CC) $(LDFLAGS) -o $(EXEC) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(EXEC) $(OBJS)

format:
	clang-format -i -style=file *.[ch]

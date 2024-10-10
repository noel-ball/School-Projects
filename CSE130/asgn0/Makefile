#------------------------------------------------------------------------------
#  Noel Ball njball 01/13/2024
#  Makefile for CSE 130 Programming Assignment 0
#
#  make all                 makes hello
#  make clean               removes all binaries
#  make hello               makes hello
#------------------------------------------------------------------------------

CC             = clang
FORMAT         = clang-format -i
MAIN           = hello
SOURCE         = $(MAIN).c
OBJECT         = $(MAIN).o
CFLAGS         = -Wall -Wextra -Werror -pedantic
REMOVE         = rm -f

all: $(MAIN)

clean:
	$(REMOVE) $(MAIN) $(OBJECT)

$(MAIN): $(OBJECT)
	$(CC) $(CFLAGS) $(OBJECT) -o $(MAIN)
	$(FORMAT) $(SOURCE)




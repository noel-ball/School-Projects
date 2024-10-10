#------------------------------------------------------------------------------
#  Noel Ball njball 01/24/2024
#  Makefile for CSE 130 Programming Assignment 1
#
#  make all                 makes memory
#  make clean               removes all binaries
#  make memory              makes memory
#------------------------------------------------------------------------------

CC             = clang
FORMAT         = clang-format -i
MAIN           = memory
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




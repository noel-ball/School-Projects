# Game of life

## Included Files:
life.c
universe.c
Makefile
DESIGN.pdf
WRITEUP.pdf
and associated object files

## Run Instructions:
Open terminal in folder with included files
Run the following commands;
make
./life -[arguments]
make clean

## Operation:
Running make first cleans life to ensure there are no current existing object files. Then the executable and the dependent object files are generated and formatted, and the ncurses() library is linked.

SYNOPSIS
    Conway's Game of Life

USAGE
    ./life_amd64 -tsnioh

OPTIONS
    -t             Create your universe as a toroidal\n"
    -s             Silent - do not use animate the evolution\n"
    -n {number}    Number of generations [default: 100]\n"
    -i {file}      Input file [default: stdin]\n"
    -o {file}      Output file [default: stdout]"


Depending on the argument an input file sets live cells in a universe, that may be set to toroidal, and unless specified to be silenced ncurses() will display the generations advancing in the universe.
Once the set number of generations has been met, the final generation is output to a file.
If -H, even with other arguments, or nothing at all is included the program synopsis and usage display is the only output.

When finished running ./life, running make clean ensures the object files and executable are removed after use.

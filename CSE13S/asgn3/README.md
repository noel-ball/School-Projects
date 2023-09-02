###sorting

Included Files:
sorting.c
batcher.c
heap.c
quick.c
set.c
shell.c
stats.c
Makefile
DESIGN.pdf
WRITEUP.pdf
and associated object files

Run Instructions:
Open terminal in folder with included files
Run the following commands;
make
./sorting -[arguments]
make clean

Operation:
Running make first cleans sorting to ensure there are no current existing object files. Then the executable and the dependent object files are generated and formatted. 

SYNOPSIS
   A collection of comparison-based sorting algorithms.

USAGE
   ./sorting [-Hasbhqn:p:r:] [-n length] [-p elements] [-r seed]

OPTIONS
   -H              Display program help and usage.
   -a              Enable all sorts.
   -s              Enable Shell Sort.
   -b              Enable Batcher Sort.
   -h              Enable Heap Sort.
   -q              Enable Quick Sort.
   -n length       Specify number of array elements (default: 100).
   -p elements     Specify number of elements to print (default: 100).
   -r seed         Specify random seed (default: 13371453).


Depending on the argument input, specific functions are run to sort a random array with the option of a desired length or seed. Then the number of elements, moves made, and comparisons made, as well as the sorted array will be printed to the desired length. 
All functions are only run once despite duplicates in the input, or -a included with anything else.
If -H, even with other arguments, or nothing at all is included the program synopsis and usage display is the only output.

When finished running ./sorting, running make clean ensures the object files and executible are removed after use.

# mathlib-test

## Included Files:
mathlib-test.c
mathlib.h
bbp.c
e.c
euler.c
madhava.c
newton.c
viete.c
Makefile
DESIGN.pdf
WRITEUP.pdf

## Run Instructions:
Open terminal in folder with included files
Run the following commands;
make
./mathlib-test -(arguments)
make clean

## Operation:
Running make first cleans mathlib-test to ensure there are no current existing object files. Then the executable and the dependent object files are generated and formatted. 

./mathlib-test takes the following arguments;
  -a   Runs all tests.
  -e   Runs e test.
  -b   Runs BBP pi test.
  -m   Runs Madhava pi test.
  -r   Runs Euler pi test.
  -v   Runs Viete pi test.
  -n   Runs Newton square root tests.
  -s   Print verbose statistics.
  -h   Display program synopsis and usage.

Depending on the argument input, specific functions are run to approximate values and they print the estimated value, the math.h libraries value, and the difference between the two. 
All functions are only run once despite duplicates in the input, or -a included with anything else.
If -h is included, even with other arguments, the program synopsis and usage display is the only output.

When finished running ./mathlib-test, running make clean ensures the object files and executible are removed after use.

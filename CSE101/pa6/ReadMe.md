### BigInteger ADT with Arithmetic executable and BigIntegerTest and ListTest harnesses

Included Files:
Makefile           - Compiler for executable programs
BigInteger.cpp     - Source File for BigInteger ADT
BigInteger.h       - Header File for BigInteger ADT
List.cpp           - Source File for List ADT
List.h             - Header File for List ADT
Arithmetic.cpp     - Executable client module, takes input and output files
BigIntegerTest.cpp - Testing program using all functionalities of Matrix ADT
ListTest.cpp       - Testing program using all functionalities of List ADT

Usage:
- make                     	        makes Arithmetic
- make BigIntegerTest			makes BigIntegerTest
- make ListTest                         makes ListTest
- ./Arithmetic <input> <output>	        runs Arithmetic with specified files
- ./BigIntegerTest   		        runs tests on BigInteger functionality
- ./ListTest                            runs tests on list functionality
- make ArithmeticCheck          	tests Arithmetic for memory leaks
- make BigIntegerCheck                  tests BigIntegerTest for memory leaks
- make ListCheck                        test ListTest for memory leaks 
- make clean                    	removes binaries

###Graph ADT with FindComponent executable and GraphTest testing harness

Included Files:
Makefile - Compiler for executable programs
List.c - Source File for List ADT
List.h - Header File for List ADT
Graph.c - Source File for Graph ADT
Graph.h - Header File for Graph ADT
FindComponent.c - Executable program, takes input and output files
GraphTest.c - Testing program using all functionalities of Graph ADT

Usage:
- make                     		makes FindComponents
- make GraphTest           		makes GraphTest
-./FindComponenets <input> <output>	runs FindComponents with specified files
-./GraphTest 				runs tests on graph functionality
- make checkFind           		tests FindComponents for memory leaks
- make checkTest           		tests GraphTest for memory leaks
- make clean               		removes binaries

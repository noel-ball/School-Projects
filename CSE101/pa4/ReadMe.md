# Matrix ADT with Sparse executable and GraphTest testing harness

## Included Files:
- Makefile - Compiler for executable programs
- Matrix.c - Source File for Matrix ADT
- Matrix.h - Header File for Matrix ADT
- List.c - Source File for List ADT
- List.h - Header File for List ADT
- Sparse.c - Executable program, takes input and output files
- MatrixTest.c - Testing program using all functionalities of Matrix ADT
- ListTest.c - Testing program using all functionalities of List ADT

## Usage:
- make                     	        	makes Sparse
- make MatrixTest          	    	    makes MatrixTest
- make ListTest                         makes ListTest
-./Sparse <input> <output>	            runs Sparse with specified files
-./MatrixTest   		                runs tests on matrix functionality
- ./ListTest                            runs tests on list functionality
- make SparseCheck          	    	tests Sparse for memory leaks
- make MatrixCheck                   	tests MatrixCheck for memory leaks
- make ListCheck                        test ListTest for memory leaks 
- make clean                    		removes binaries

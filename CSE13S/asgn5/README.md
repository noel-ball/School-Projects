# SS Encyrption

## Included Files:
keygen.c
encrypt.c
decrypt.c
numtheory.c
ss.c
randstate.c
Makefile
DESIGN.pdf
WRITEUP.pdf
and associated header and object files

## Run Instructions:
Open terminal in folder with included files
Run the following commands;
make
./keygen -[arguments]
./encrypt -[arguments] or ./decrypt -[arguments]
make clean

## Operation:
Running make first cleans life to ensure there are no current existing object files. Then the executable and the dependent object files are generated and formatted, and the libraries are linked.
Keygen generates the public and private files and keys to be stored. 
Encrypt uses keys to encrypt message from input file to output file. 
Decrypt uses keys to decrypt message from input file to output file. 


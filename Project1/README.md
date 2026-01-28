# Line Reversing Utility

## Description
reverse is a simple Unix-style command-line program written in C.
It reads lines of text from an input stream and prints them in reverse order.

## Usage
./reverse	Reads from standard input and writes to standard output

./reverse input.txt	Reads from a file and writes to standard output

./reverse input.txt output.txt	Reads from a file and writes to another file


### Example
Input file:
hello
this
is
a file

Output:
a file
is
this
hello


## Make commands
make (to build the program)

make clean (remove the build)


## Run
cd Project1/build/

./reverse input.txt output.txt


### Error Handling
The program handles the following error cases:
- Too many arguments
- Input and output files are the same
- File cannot be opened
- Memory allocation failure
Errors are printed to stderr, and the program exits with status code 1.

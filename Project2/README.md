# Basic UNIX Utilities

## Description
This project consists of four small UNIX-style command-line utilities written in C:

my-cat – prints file contents

my-grep – searches for lines containing a string

my-zip – compresses files using run-length encoding (RLE)

my-unzip – decompresses files created by my-zip

Each program is implemented as a single .c file and compiled separately.


## Usage
From the Project2 directory:

make

This creates the executables inside the build/ directory:

build/my-cat

build/my-grep

build/my-zip

build/my-unzip

To remove all compiled files:

make clean

All programs are compiled with:
-Wall -Werror -g

### my-cat
Usage: ./my-cat file1 [file2 ...]
Prints each file in order. If no files are given, exits successfully without output
On file open failure prints an error.

### my-grep
Usage: ./my-grep searchterm [file ...]
Searches for a string within lines of input and prints matching lines.
Case-sensitive matching. Uses getline() to support arbitrarily long lines. If no files are given, reads from stdin.

### my-zip and my-unzip
Usage:
./my-zip file1 [file2 ...] > output.z
./my-unzip file.z

(my-zip) Compresses files using Run-Length Encoding (RLE). Instead of storing repeated characters individually, it stores:
[count (4-byte int)][character (1 byte)]
Output is written to stdout. Multiple input files are compressed into one continuous stream

(my-unzip) Decompresses files created by my-zip. Outputs decompressed data to stdout. Multiple compressed files can be given.

# Parallel Compression

### Description
This project implements a parallel file compression and decompression system using POSIX threads and memory-mapped I/O. It is based on run-length encoding (RLE) and is designed to efficiently compress one or multiple files by utilizing multiple CPU cores.

The project consists of two programs:
pzip: Compresses files in parallel.
punzip: Decompresses files in parallel.
(punzip2: simpler version of punzip that doesn't use threads)

NOTE: punzip doesn't currently work, use punzip2 instead!
NOTE: The RLE format is architecture-independent if compiled and run on the same system. Cross-platform portability may require handling endianness!

### Features
Parallel Compression: Input files are combined and split into chunks, each compressed by a separate thread.
(Parallel Decompression: Compressed entries are split across threads and expanded independently.)
Memory-Mapped File I/O (mmap): Zero-copy access to input files for efficient reading.
Binary RLE Format: Each run is stored as a 4-byte integer followed by a single character.
Multi-Core Utilization: Automatically detects the number of CPU cores and divides work evenly.
Boundary-Safe Compression: Ensures runs that span thread chunks are merged correctly.
Multiple File Support: Handles multiple input files as one continuous stream.

The tests/ folder includes some test files that were used for checking the program.

### Usage
./pzip file1.txt file2.txt > compressed.z
Multiple input files are concatenated logically and compressed in parallel.
The output is written in binary RLE format to standard output.

./punzip compressed.z > decompressed.txt
Reads one or more RLE-compressed files and outputs the original content.
Decompression is parallelized across threads for performance.

#### Compilation
make
This produces the executables:
pzip
punzip

Clean build files with:
make clean
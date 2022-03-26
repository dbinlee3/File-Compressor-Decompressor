Name: Dong Bin Lee
Date: 2/25/2022

This project contains two programs that are meant to compress/decompress a given text file.

A Huffmantree data structure is implemented and used to create an efficient way to encode
the given text file in order to compress it to a much smaller size.

A Priority Queue data structure is used in purpose to construct the Huffman Tree, by
ordering Nodes carrying symbols with the lowest frequencies. This way, an efficient
encoding is implemented with the most frequent symbols with the least bits to encode.

A header for the output file is built by using an efficient encoding for the Huffman Tree
by writing bytes to the output stream. This header will be used for uncompressing a compressed
file in order to rebuild the Huffman Tree in the uncompressing process.


/******************************/
/******   How To Use:    ******/
/******************************/

There are two programs, one to compress a given text file and the other to decompress a compressed file.
To use the compress program, use:
./compress <file_to_compress> <output>

To use the uncompress program, use:
./uncompress <compressed_file> <uncompressed_file>


The result from this project yields an <uncompressed_file> that is 100% identical to the <file_to_compress>.

The folder "example_files" contains example text files to test the two programs.
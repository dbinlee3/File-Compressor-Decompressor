#include <iostream>
#include <vector>
#include "HCTree.hpp"
#include "Helper.hpp"
using namespace std;

/**
 * Usage: ./uncompress <compressed_file> <uncompressed_file>
 * 
 * This file uncompresses a <compressed_file> as input, using an efficient
 * algorithm to decode the header to create a Huffman Tree data structure,
 * in order to uncompress the encoded message to write the output to
 * <uncompressed_file>.
*/

/**
 * A recursive algorithm which decodes the header from <compressed_file> in order
 * to create a Huffman Tree data structure bottom-up.
 * 
 * @param myHuffman - A reference to an HCTree type, which is the Huffman Tree
 * @param inputStream - A reference to a FancyInputStream type which contains the input stream
 * @return HCNode *- Returns a new node containing a decoded symbol to be added to the Huffman Tree
 */
HCNode *decodeHeader(HCTree &myHuffman, FancyInputStream &inputStream){

    //If the current byte is 1, read the next byte as a symbol
    //and return a new node containing the symbol.
    if (  inputStream.read_byte() == 1 ){

        unsigned char myChar = inputStream.read_byte();
        return new HCNode (1, myChar);
    }
    //Else, recursively call and build the Huffman tree bottom-up.
    else{
        HCNode *leftChild = decodeHeader(myHuffman, inputStream);
        HCNode *rightChild = decodeHeader(myHuffman, inputStream);

        HCNode *parent = new HCNode(0, leftChild->symbol);
        leftChild->p = parent;
        rightChild->p = parent;
        parent->c0 = leftChild;
        parent->c1 = rightChild;

        return parent;
    }
}

int main(int argc, char** argv){
    
    //Check arguments
    if (argc != 3){
        cerr << "Invalid usage. Please use format './uncompress <compressed> <uncompressed_file>'" << endl;
        exit(1);
    }

    //Open the input file for reading
    FancyInputStream inputStream(argv[1]); //Create the input stream

    //Check if the input file is empty:
    if (inputStream.filesize() == 0){
        FancyOutputStream emptyFile(argv[2]);
        return 0;
    }

    HCTree *myHuffmanTree = new HCTree();

    //Read the first byte of <compressed_file> to record the total quantity
    //of frequencies of each symbol of the original file.
    int numFreqs = inputStream.read_int(); 

    //Build the huffman tree
    myHuffmanTree->root = decodeHeader(*myHuffmanTree, inputStream);

    FancyOutputStream outputStream(argv[2]); //Create the output stream for writing
    unsigned char currChar;

    //Decode the encryption using the built Huffman tree and write to <uncompressed_file>
    for (int i = 0; i < numFreqs; i++){
         currChar = myHuffmanTree->decode(inputStream);
         outputStream.write_byte(currChar);
    }

    delete myHuffmanTree;
    return 0;
}

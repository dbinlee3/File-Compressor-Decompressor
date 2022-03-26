#include <iostream>
#include <vector>
#include "HCTree.hpp"
#include "Helper.hpp"
using namespace std;

/**
 * Usage: ./compress <original_file> <compressed_file>
 * 
 * This file compresses an <original_file> with an efficient encoding using
 * a Huffman Tree data structure. It will write to the output <compresseD_file>
 * an efficient header, as well each encoded symbol from the input that is
 * designated from the Huffman Tree.
*/

/**
 * A recursive algorithm which encodes the header for the compressed output file.
 * If the current node is a leaf, it will write 1 followed by the node's symbol to the header.
 * If the current node is not a leaf, it will recursively call on its children
 * in a pre-traversal order.
 * 
 * @param node - A pointer to an HCNode type which is the current node
 * @param out - A reference to a FancyOutputStream type which contains the output stream
 */
void encodeHeader(HCNode *node, FancyOutputStream & out){

    //If the current node is a leaf:
    if ( (node->c0 == nullptr) && (node->c1 == nullptr) ){

        out.write_byte(1); //Write 1 to the header
        out.flush_bitwise();
        out.write_byte(node->symbol); //Write Node's symnbol to the header
    }
    else{

        out.write_byte(0); //Write 0 to the header

        //Pre-traversal recursive calls to children
        encodeHeader(node->c0, out);
        encodeHeader(node->c1, out);
    }
}

int main(int argc, char** argv){

    //Check arguments
    if (argc != 3){
        cerr << "Invalid usage. Please use format './compress <original_file> <compressed_file>'" << endl;
        exit(1);
    }

    //Create the input stream:
    FancyInputStream inputStream(argv[1]);

    //Check if the input file is empty:
    if (inputStream.filesize() == 0){
        FancyOutputStream emptyFile(argv[2]);
        return 0;
    }

    vector<int> freqs (256, 0); //Creates a vector of size 256 with 0's in it
    int currByte;
    int totalFreqs = 0;
    
    //Until we've reached EOF, read + increment each byte
    while ( (currByte = inputStream.read_byte()) != -1 ){
        freqs[currByte]++; //Increment frequency
        totalFreqs++;
    }

    //Build the HCTree w/ freqs vector
    HCTree *myHuffmanTree = new HCTree();
    myHuffmanTree->build(freqs);

    //Create the output stream
    FancyOutputStream outputStream(argv[2]);

    //Write to the header
    outputStream.write_int(totalFreqs); //Write to header initial frequency of num nodes
    encodeHeader(myHuffmanTree->root, outputStream);

    //Reset the inputStream to read again
    inputStream.reset();

    //Until we've reached EOF, read + encode each byte to the HCTree
    while ( (currByte = inputStream.read_byte()) != -1 ){
        
        myHuffmanTree->encode((unsigned char)currByte, outputStream);
    }

    delete myHuffmanTree;
    return 0;
}
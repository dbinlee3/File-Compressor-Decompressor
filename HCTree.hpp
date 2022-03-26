#ifndef HCTREE_HPP
#define HCTREE_HPP
#include <queue>
#include <vector>
#include <stack>
#include <fstream>
#include "Helper.hpp"
using namespace std;

/**
 * This is a class for a Huffman Tree data structure
 */
class HCTree {

    private:
        vector<HCNode*> leaves; //A vector of node pointers which will be the leaves of the tree

    public:

        HCNode* root; //A pointer to the root of the tree

        /**
         * Constructor, which initializes everything to null pointers
         */
        HCTree() : root(nullptr) {
            leaves = vector<HCNode*>(256, nullptr);
        }

        /**
         * This is the deconstructor for the HCTree class
         */
        ~HCTree();

        /**
         * This function builds a Huffman Tree data structure using an input which contains the
         * frequencies of each symbol of the input text file to be compressed.
         * 
         * @param freqs - A vector of <int> types which contain the frequencies of every symbol of the
         * input file.
         */
        void build(const vector<int>& freqs);

        /**
         * This function encodes a single given char type using the built Huffman Tree
         * and writes the encoded bit(s) to the output stream.
         * 
         * @param symbol - A char type which is the symbol to be encoded
         * @param out - A reference to a FancyOutputStream type, which contains the output stream
         */
        void encode(unsigned char symbol, FancyOutputStream & out) const;

        /**
         * This function decodes the next bit(s) of the inputstream using the Huffman Tree
         * in order to return the next character for the decoded file.
         * 
         * @param in - A reference to a FancyInputStream type, which contains the input stream
         * @return unsigned char - An unsigned char type which is the decoded character
         */
        unsigned char decode(FancyInputStream & in) const;
};
#endif
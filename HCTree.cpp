#include "HCTree.hpp"

/**
 * This function builds a Huffman Tree data structure using an input which contains the
 * frequencies of each symbol of the input text file to be compressed.
 * 
 * @param freqs - A vector of <int> types which contain the frequencies of every symbol of the
 * input file.
 */
void HCTree::build(const vector<int>& freqs){

    HCNode *currNode; //Create a reference HCNode

    //Create the "Forest" containing single-node trees w/ frequencies
    priority_queue<HCNode *, vector<HCNode *>, HCNodePtrComp> forest;

    //Populate the forest using the freqs vector
    for (long unsigned int i = 0; i < freqs.size(); i++){

        //If count of current symbol is greater than 0:
        if (freqs[i] > 0){

            //Create a new node:
            currNode = new HCNode(freqs[i], (char)i);

            //Add it to the HCTree:
            this->leaves[i] = currNode;

            //Add the newly created node into the "Forest":
            forest.push(currNode);
        }
    }

    //Local Variables
    HCNode *tree1;
    HCNode *tree2;
    HCNode *parent;

    //Build the Huffman Tree using the forest Priority Queue data structure
    while (forest.size() > 1){

        //Remove top two entries
        tree1 = forest.top();
        forest.pop();
        tree2 = forest.top();
        forest.pop();

        //Create new parent node
        parent = new HCNode(tree1->count + tree2->count, tree1->symbol);

        //Set parent, c0, and c1 fields:
        tree1->p = parent;
        tree2->p = parent;
        parent->c0 = tree1;
        parent->c1 = tree2;

        //Push the parent into the queue
        forest.push(parent);
    }

    //Set the root of the HC:
    this->root = forest.top();
}

/**
 * This function encodes a single given char type using the built Huffman Tree
 * and writes the encoded bit(s) to the output stream.
 * 
 * @param symbol - A char type which is the symbol to be encoded
 * @param out - A reference to a FancyOutputStream type, which contains the output stream
 */
void HCTree::encode(unsigned char symbol, FancyOutputStream & out) const{

    stack<int> *myStack = new stack<int>(); //Create the stack

    //Set currNode to the right symbol
    HCNode *currNode = this->leaves[(int)symbol];
    HCNode *tempNode;

    //While the currNode has a parent:
    while (currNode->p != nullptr){

        tempNode = currNode->p; //Set tempNode to currNode's parent

        //If currNode was the left child, push 0
        if (tempNode->c0 == currNode){
            myStack->push(0);
        }

        //Else, push 1
        else if (tempNode->c1 == currNode){
            myStack->push(1);
        }

        currNode = currNode->p;
    }

    //Pop each element in stack, and write it to OutputStream
    while (myStack->size() != 0){
        out.write_bit( myStack->top() );
        myStack->pop();
    }

    delete(myStack);
}

/**
 * This function decodes the next bit(s) of the inputstream using the Huffman Tree
 * in order to return the next character for the decoded file.
 * 
 * @param in - A reference to a FancyInputStream type, which contains the input stream
 * @return unsigned char - An unsigned char type which is the decoded character
 */
unsigned char HCTree::decode(FancyInputStream & in) const{

    HCNode *currNode = this->root; //Set currNode

    //While the current Node has children:
    while ( (currNode->c0 != nullptr) && (currNode->c1 != nullptr) ){

        //Read the next 0/1 from the InputStream
        int nextNum = in.read_bit();

        //Progress down the tree
        if (nextNum == 0){
            currNode = currNode->c0;
        }
        else if (nextNum == 1){
            currNode = currNode->c1;
        } 
        else if (nextNum == -1){
            return '\0';
        }

    }
    return currNode->symbol;
}

/**
 * This is a helper function which deconstructs the Huffman Tree recursively
 * 
 * @param currNode - A pointer to a HCNode type which is the current Node being called
 */
void deleteRecursive(HCNode *currNode)
{
    if (currNode){
        deleteRecursive(currNode->c0);
        deleteRecursive(currNode->c1);
        delete currNode;
    }
}

/**
 * This is the deconstructor for the HCTree class
 */
HCTree::~HCTree() {
    deleteRecursive(this->root);\
}



/*
 * Name: Paul Nguyen
 *
 * This file provides a skeleton for a huffman tree. For students, feel free
 * to implement the tree in HCTree.cpp (you can create a new file).
 */

#ifndef HCTREE_HPP
#define HCTREE_HPP
#include <queue>
#include <vector>
#include <fstream>
#include "Helper.hpp"
using namespace std;

/**
 * A Huffman Code Tree class
 */
class HCTree {
private:
    HCNode* root;
    vector<HCNode*> leaves;

    /**
     * Method to help destrucotr deallocate any dynamic memory
     * @param Node The node to be deleted
     */
    void destructorHelper(HCNode *Node);

    /**
     * Method to help encode recursively iterate though tree 
     * bottom to top
     * @param currNode The node to go up the tree on
     * @param output stream to be written to
     */
    void encodeHelper(HCNode *currNode, FancyOutputStream & out) const;

public:
    /**
     * Constructor, which initializes everything to null pointers
     */
    HCTree() : root(nullptr) {
        leaves = vector<HCNode*>(256, nullptr);
    }

    ~HCTree();

    /**
     * Use the Huffman algorithm to build a Huffman coding tree.
     * PRECONDITION:  freqs is a vector of ints, such that freqs[i] is the
     *                frequency of occurrence of byte i in the input file.
     * POSTCONDITION: root points to the root of the trie, and leaves[i]
     *                points to the leaf node containing byte i.
     *
     * @param freqs frequency vector
     */
    void build(const vector<unsigned int>& freqs);

    /**
     * Write to the given FancyOutputStream the sequence of bits coding the
     * given symbol.
     * PRECONDITION: build() has been called, to create the coding tree,
     *               and initialize root pointer and leaves vector.
     *
     * @param symbol symbol to encode
     * @param out output stream for the encoded bits
     */
    void encode(unsigned char symbol, FancyOutputStream & out) const;

    /**
     * Encode method when their is only one unique charcter in file.
     * writes a single binary 1 to the output file.
     * PRECONDITION: build() has been called, to create the coding tree,
     *               and initialize root pointer and leaves vector.
     *
     * @param out output stream for the encoded bits
     */
    void encodeOne(FancyOutputStream & out) const;

    /**
     * Return symbol coded in the next sequence of bits from the stream.
     * PRECONDITION: build() has been called, to create the coding tree, and
     *               initialize root pointer and leaves vector.
     *
     * @param in input stream to find encoded bits
     * @return a single char decoded from the input stream
     */
    unsigned char decode(FancyInputStream & in) const;
};
#endif // HCTREE_HPP

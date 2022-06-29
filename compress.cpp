/*
 * Name: Paul Nguyen
 *
 *
 * This file implement the compression of a file.
 * Takes in a file and ourputs the encoded binary file.
 * Format is: ./compress <original_file> <compressed_file>
 */
#include "Helper.hpp"
#include "HCTree.hpp"

int main(int argc, char* argv[]) {

    //
    //check if arguments correct
    //
    if(argc != 3){
        fprintf(stderr, "Input error. Format:\n");
        fprintf(stderr, 
                "./compress <original_file> <compressed_file>\n");
        return 1;
    }


    //
    // open file and check
    //
    FancyInputStream inFile(argv[1]); // object on stack
    if(!inFile.good()){
        fprintf(stderr, "open file error\n");
        return 1;
    }


    //
    // read from file
    //
    vector<unsigned int> freqs(256, 0); // keep track of frequecies

    // fill vector with freq from data in file
    while(true){
        unsigned char data = inFile.read<unsigned char>(); // read data
        if (!inFile.good()) break; // check EOF
        freqs[data]++;
    }

    // checks number of unique elements
    int numUnique = 0; // tracks number of unique charcters
    for (unsigned int i = 0; i < freqs.size(); i++){
        if (freqs[i] > 0){
            numUnique++;
        }
    }


    // 
    // open file for writing and check
    //
    FancyOutputStream outFile(argv[2]); // object on stack
    if(!outFile.good()){
        fprintf(stderr, "open file error\n");
        return 1;
    }

    // no unique elements means empty file
    if (numUnique == 0){
        return 0;
    }


    // 
    // write header
    //
    for (unsigned int i = 0; i < freqs.size(); i++){
        outFile.write<unsigned short>(freqs[i]);
    }


    // 
    // build huffman tree
    //
    HCTree tree = HCTree(); // creates tree object on stack
    if (numUnique != 1){ // check at least 1 unique char
        tree.build(freqs);
    }
    

    // 
    // move to begining of input file to read again
    //
    inFile.reset();


    //
    // translate each byte in file to huffman code
    //
    while(true){
        unsigned char data = inFile.read<unsigned char>(); // read data
        if (!inFile.good()) break; // check EOF
        // special case if there is only one unique char
        if (numUnique == 1){
            tree.encodeOne(outFile);
        }
        // normal case
        else{
            tree.encode(data, outFile);
        }
    }

    // files default close since created on stack

    // c++ default return 0 for success
}

/*
 * Name: Paul Nguyen
 *
 *
 * This file implement the decompression of a file.
 * Takes in a binary file and ourputs the decoded file.
 * Format is: ./decompress <compressed_file> <decompressed_file>
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
                "./decompress <compressed_file> <decompressed_file>\n");
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
    unsigned int numChar = 0; // total frequency of all chars

    // fill vector with freq from data in file
    for(long unsigned int i = 0; i < freqs.size(); i++){
        unsigned short data = inFile.read<unsigned short>(); // read data
        freqs[i] = data;
    }

    // checks number of unique elements total frequency
    int numUnique = 0; // tracks number of unique charcters
    for (unsigned int i = 0; i < freqs.size(); i++){
        if (freqs[i] > 0){
            numUnique++;
            numChar+= freqs[i];
        }
    }


    //
    // open file for writing and check
    //
    FILE *outFile; // holds the out file
    outFile = fopen(argv[2], "w");
    if(outFile == NULL){
        fprintf(stderr, "open file error\n");
        return 1;
    }

    // no unique elements means empty file
    if (numUnique == 0){
        fclose(outFile);
        return 0;
    }


    //
    // build huffman tree
    //
    HCTree tree = HCTree(); // creates tree object on stack
    // check at least 1 unique char
    if (numUnique != 1 ){
        tree.build(freqs);
    }

    // 
    // translating entire input file
    //
    unsigned char data; // stores read data
    
    // if their is only one unique char, find what it is
    if (numUnique == 1 ){
        // find what the unqiue char is
        for (unsigned int i = 0; i < freqs.size(); i++){
            if (freqs[i] > 0) data = (unsigned char)i;
        }
    }

    // iterate for total number of chars
    for(unsigned int i = 0; i < numChar; i++){ 
        // only one unique char case
        if (numUnique == 1 ){
            char bit = inFile.read_bit(); // get bit
            // 1 bit is found so print char
            if (bit){
                fprintf(outFile, "%c", data);
            }  
            if (!inFile.good()) break; // check EOF
        }

        //normal case
        else{
            data = tree.decode(inFile); // read data
            if (!inFile.good()) break; // check EOF
            // write a single character to output file
            fprintf(outFile, "%c", data);
        } 
    }


    //
    // close any file that don't close by default
    //
    fclose(outFile);


    // c++ default return 0 for success
}

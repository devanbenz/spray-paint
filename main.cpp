#include <iostream>

#include "src/huffman.h"

// Huffman encoding
// lossless data compression algorithm

// Read the text and determine the frequency of each character occurring.
// Build the binary tree from the frequencies.
// Generate the prefix-code table from the tree.
// Encode the text using the code table.
// Encode the tree - we’ll need to include this in the output file so we can decode it.
// Write the encoded tree and text to an output field

void usage() {
    std::cout << "Usage: ./spray_paint <flag> <filename> <output>\n"
              << "\n"
              << "spray_paint is a file compression and decompression tool.\n"
              << "\n"
              << "Arguments:\n"
              << "  <flag>       d or c for [d]ecompress or [c]ompress.\n"
              << "  <filename>   The name of the file to compress or decompress.\n"
              << "  <output>     The name of the output file for compression or decompression."
              << "\n"
              << "Examples:\n"
              << "  ./spraypaint c example.txt example.spz\n"
              << "  ./spraypaint d example.spz example.txt\n\n";
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        usage();
        return 0;
    }

    auto flag = argv[1];
    auto input = argv[2];
    auto output= argv[3];

    if (strcmp(flag, "d")  != 0 && strcmp(flag, "c") != 0) {
        usage();
        return 0;
    }

    SprayPaintTree tree;
    auto spf = SprayPaintFile(std::move(tree), output, input);

    if (strcmp(flag, "d") == 0) {
        spf.read();
    } else if (strcmp(flag, "c") == 0) {
        spf.write();
    }

    return 0;
}

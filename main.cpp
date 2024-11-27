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
    std::cout << "Usage: ./spraypaint <flag> <filename> <output>\n"
              << "\n"
              << "spraypaint is a file compression and decompression tool.\n"
              << "\n"
              << "Arguments:\n"
              << "  <flag>       d or c for [d]ecompress or [c]ompress.\n"
              << "  <filename>   The name of the file to compress or decompress.\n"
              << "  <output>     The name of the output file for compression"
              << "\n"
              << "Example:\n"
              << "  ./spraypaint example.txt example.spz\n\n"
              << "Note:\n"
              << "  - If the file is already compressed, spraypaint will attempt to decompress it.\n"
              << "  - If the file is uncompressed, spraypaint will compress it.\n";
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        usage();
        return 0;
    }

    auto flag = argv[1];
    auto filename = argv[2];
    auto output= argv[3];

    if (strcmp(flag, "d")  != 0 || strcmp(flag, "c") != 0) {
        usage();
        return 0;
    }

    std::ifstream input_file(filename);
    if (!input_file.is_open()) {
        throw std::runtime_error("Invalid file. Please make sure the file exists.");
    }

    auto char_map = build_char_map(std::move(input_file));
    SprayPaintTree tree;

    tree.register_charset(char_map);
    tree.build();

    std::ofstream outfile (output,std::ofstream::binary);
    tree.serialize(outfile);
    outfile.close();

    return 0;
}

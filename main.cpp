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

int main() {
    std::ifstream input_file("/Users/devan/Documents/Projects/spray-paint/135-0.txt");
    assert(input_file.is_open());
    assert(input_file.good());

    auto char_map = build_char_map(std::move(input_file));

    return 0;
}

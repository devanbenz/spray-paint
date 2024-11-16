#include "huffman.h"

std::unordered_map<char, int> build_char_map(std::ifstream is) {
    std::unordered_map<char, int> char_map;
    char c;
    while (is.get(static_cast<char &>(c))) {
        auto it = char_map.find(c);
        if (it == char_map.end()) {
            char_map.insert(std::make_pair(c, 1));
        } else {
            it->second = ++it->second;
        }
    }

    assert(!char_map.empty());
    is.close();
    assert(!is.is_open());

    return char_map;
}

std::vector<std::pair<char, int>> build_set(std::unordered_map<char, int> &src) {
    std::vector<std::pair<char, int>> pairs;
    pairs.reserve(src.size());

    for (auto i : src) {
        pairs.emplace_back(i);
    }

    std::sort(pairs.begin(), pairs.end(), [=](std::pair<char, int> &a, std::pair<char, int> &b){
        return a.second < b.second;
    });

    return pairs;
}

void SprayPaintNode::insert_node(SprayPaintNode *node) {
    if (node == nullptr) {
        throw std::runtime_error("failed to insert node; cannot insert nil value.");
    }

    auto head = (*this);
    auto tail = (*this);

    if (head.weight_ < node->weight_) {
        // swap nodes - head will become the incoming node and the tail is now the head


        // Since the source node's weight is less than the new node it will need to become an arm of the new node.
        // Binary trees require that the left arm is lesser so the source node will need to become the left arm.

        // Traverse tree if node is less than
    }
}
#include "huffman.h"
#include <bitset>

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

void SprayPaintTree::build() {
    if (!this->charset_.has_value()) {
        throw std::runtime_error("charset is not registered; please use .register_charset() to register a character set for encoding.");
    }

    auto charset = this->charset_.value();
    auto min_heap = MinHeap<SprayPaintTree>(charset.size());

    /* First we will build the min heap
     * and then loop through all min_heap values
     * adding them to the max_heap. This will
     * ensure that not too much memory is used during compression.*/
    for (auto i : charset) {
        auto nt = SprayPaintTree(i.second, i.first);
        min_heap.put(nt);
    }

    /* For now I will assume that there are always two pop'ed variables available
     * I will adjust if needed to account for odd numbered heaps.*/
    while (min_heap.size() > 1) {
        auto tmp1 = min_heap.pop().root().value();
        auto tmp2 = min_heap.pop().root().value();
        auto tmp3 = SprayPaintTree(std::move(tmp1), std::move(tmp2));
        min_heap.put(std::move(tmp3));
    }
    auto tree = std::move(min_heap.pop().root_);
    this->root_.swap(tree);
}

SprayPaintTree read_encoder(const std::string& file_name) {
    SprayPaintTree data;
    std::ifstream file(file_name);

}

void write_encoder(const SprayPaintTree& data, const std::string& file_name) {
    std::ofstream file(file_name);
    file << data;
}

// todo: implement DFS to get path of each leaf node
void generate_encodings(std::unique_ptr<SprayPaintNode> node, std::unordered_map<char, std::string>& map, const std::string& path) {
    if (node == nullptr) {
        return;
    }

    if (node->leaf()) {
        map[node->value()] = path;
        return;
    }

    generate_encodings(std::move(node->left()), map, path + "0");
    generate_encodings(std::move(node->right()), map, path + "1");
}

/* To encode data depth first seach will be used
 * to generate a string of 0's and 1's associated with
 * each individual character. This may be a lot of work :thinking:
 * It's currently my best idea though.*/
void SprayPaintTree::encode(const std::string& path) {
    if (!this->root().has_value()) {
        throw std::runtime_error("There is not root value. Please build a huffman code tree using build() before trying to encode data.");
    }
    std::unordered_map<char, std::string> ret;
    auto cloned_tree = this->clone();
    generate_encodings(std::move(cloned_tree->root_), ret, "");


}


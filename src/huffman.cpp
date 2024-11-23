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

SprayPaintTree SprayPaintTree::build() {
    if (!this->charset_.has_value()) {
        throw std::runtime_error("charset is not registered; please use .register_charset() to register a character set for encoding.");
    }

    auto charset = this->charset_.value();
    auto min_heap = MinHeap<SprayPaintTree>(charset.size());

    /* First we will build the min heap
     * and then loop through all min_heap values
     * adding them to the max_heap. This will
     * ensure that not too much memory is used during
     * compression.
     * */
    for (auto i : charset) {
        auto nt = SprayPaintTree(i.second, i.first);
        min_heap.put(std::move(nt));
    }

    /* For now I will assume that there are always two pop'ed variables available
     * I will adjust if needed to account for odd numbered heaps.*/
    while (min_heap.size() > 1) {
        auto tmp1 = min_heap.pop().root().value();
        auto tmp2 = min_heap.pop().root().value();
        auto tmp3 = SprayPaintTree(std::move(tmp1), std::move(tmp2));
        min_heap.put(std::move(tmp3));
    }

    auto tree = min_heap.pop();
    return tree;
}
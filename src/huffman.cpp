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
    auto heap = MaxHeap<SprayPaintTree>(charset.size());

    for (auto i : charset) {
        auto nt = SprayPaintTree(i.second, i.first);
        heap.put(std::move(nt));
    }

    while (heap.size() > 1) {
        std::cout << heap.pop().root()->weight() << "\n";
    }

    return SprayPaintTree(5, 'B');
}
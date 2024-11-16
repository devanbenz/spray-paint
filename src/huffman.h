#pragma once

#include <unordered_map>
#include <fstream>
#include <cassert>
#include <algorithm>
#include <stdexcept>
#include <iostream>
#include <optional>

std::unordered_map<char, int> build_char_map(std::ifstream);

std::vector<std::pair<char, int>> build_set(std::unordered_map<char, int> &src);

class SprayPaintNode {
public:
    SprayPaintNode(int weight, std::optional<char> val, bool leaf) :
        weight_(weight), val_(val), leaf_(leaf), left_(nullptr), right_(nullptr) {};

    void insert_node(SprayPaintNode* node);

    [[nodiscard]] bool is_leaf() const {
        return leaf_;
    };

    [[nodiscard]] char value() const {
        if (val_.has_value()) {
            return val_.value();
        }

        throw std::runtime_error("attempting to access a None value");
    }

    [[nodiscard]] int weight() const {
        return weight_;
    }
private:
    int weight_;
    std::optional<char> val_;
    bool leaf_;
    SprayPaintNode* left_;
    SprayPaintNode* right_;
};
